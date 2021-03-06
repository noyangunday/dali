/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib> // rand
#include "shared/view.h"

#include <dali/dali.h>
#include <dali-toolkit/dali-toolkit.h>
#include <dali/devel-api/images/atlas.h>
#include <dali/devel-api/rendering/cull-face.h>

using namespace Dali;
using namespace Dali::Toolkit;

namespace
{

enum AllImagesLayouts
{
  SPIRAL_LAYOUT,
  DEPTH_LAYOUT,
  GRID_LAYOUT,

  NUMBER_OF_LAYOUTS
};

const char* IMAGE_PATHS[] = {
    DALI_IMAGE_DIR "gallery-medium-1.jpg",
    DALI_IMAGE_DIR "gallery-medium-2.jpg",
    DALI_IMAGE_DIR "gallery-medium-3.jpg",
    DALI_IMAGE_DIR "gallery-medium-4.jpg",
    DALI_IMAGE_DIR "gallery-medium-5.jpg",
    DALI_IMAGE_DIR "gallery-medium-6.jpg",
    DALI_IMAGE_DIR "gallery-medium-7.jpg",
    DALI_IMAGE_DIR "gallery-medium-8.jpg",
    DALI_IMAGE_DIR "gallery-medium-9.jpg",
    DALI_IMAGE_DIR "gallery-medium-10.jpg",
    DALI_IMAGE_DIR "gallery-medium-11.jpg",
    DALI_IMAGE_DIR "gallery-medium-12.jpg",
    DALI_IMAGE_DIR "gallery-medium-13.jpg",
    DALI_IMAGE_DIR "gallery-medium-14.jpg",
    DALI_IMAGE_DIR "gallery-medium-15.jpg",
    DALI_IMAGE_DIR "gallery-medium-16.jpg",
    DALI_IMAGE_DIR "gallery-medium-17.jpg",
    DALI_IMAGE_DIR "gallery-medium-18.jpg",
    DALI_IMAGE_DIR "gallery-medium-19.jpg",
    DALI_IMAGE_DIR "gallery-medium-20.jpg",
    DALI_IMAGE_DIR "gallery-medium-21.jpg",
    DALI_IMAGE_DIR "gallery-medium-22.jpg",
    DALI_IMAGE_DIR "gallery-medium-23.jpg",
    DALI_IMAGE_DIR "gallery-medium-24.jpg",
    DALI_IMAGE_DIR "gallery-medium-25.jpg",
    DALI_IMAGE_DIR "gallery-medium-26.jpg",
    DALI_IMAGE_DIR "gallery-medium-27.jpg",
    DALI_IMAGE_DIR "gallery-medium-28.jpg",
    DALI_IMAGE_DIR "gallery-medium-29.jpg",
    DALI_IMAGE_DIR "gallery-medium-30.jpg",
    DALI_IMAGE_DIR "gallery-medium-31.jpg",
    DALI_IMAGE_DIR "gallery-medium-32.jpg",
    DALI_IMAGE_DIR "gallery-medium-33.jpg",
    DALI_IMAGE_DIR "gallery-medium-34.jpg",
    DALI_IMAGE_DIR "gallery-medium-35.jpg",
    DALI_IMAGE_DIR "gallery-medium-36.jpg",
    DALI_IMAGE_DIR "gallery-medium-37.jpg",
    DALI_IMAGE_DIR "gallery-medium-38.jpg",
    DALI_IMAGE_DIR "gallery-medium-39.jpg",
    DALI_IMAGE_DIR "gallery-medium-40.jpg",
    DALI_IMAGE_DIR "gallery-medium-41.jpg",
    DALI_IMAGE_DIR "gallery-medium-42.jpg",
    DALI_IMAGE_DIR "gallery-medium-43.jpg",
    DALI_IMAGE_DIR "gallery-medium-44.jpg",
    DALI_IMAGE_DIR "gallery-medium-45.jpg",
    DALI_IMAGE_DIR "gallery-medium-46.jpg",
    DALI_IMAGE_DIR "gallery-medium-47.jpg",
    DALI_IMAGE_DIR "gallery-medium-48.jpg",
    DALI_IMAGE_DIR "gallery-medium-49.jpg",
    DALI_IMAGE_DIR "gallery-medium-50.jpg",
    DALI_IMAGE_DIR "gallery-medium-51.jpg",
    DALI_IMAGE_DIR "gallery-medium-52.jpg",
    DALI_IMAGE_DIR "gallery-medium-53.jpg",
};

const unsigned int NUM_IMAGES = sizeof(IMAGE_PATHS) / sizeof(char*);

const unsigned int IMAGE_WIDTH = 256;
const unsigned int IMAGE_HEIGHT = 256;
const unsigned int NUM_IMAGE_PER_ROW_IN_ATLAS = 8;

const char* BACKGROUND_IMAGE( "" );
const char* TOOLBAR_IMAGE( DALI_IMAGE_DIR "top-bar.png" );
const char* EDIT_IMAGE( DALI_IMAGE_DIR "icon-edit.png" );
const char* EDIT_IMAGE_SELECTED( DALI_IMAGE_DIR "icon-edit-selected.png" );
const char* SPIRAL_LAYOUT_IMAGE( DALI_IMAGE_DIR "icon-item-view-layout-spiral.png" );
const char* SPIRAL_LAYOUT_IMAGE_SELECTED( DALI_IMAGE_DIR "icon-item-view-layout-spiral-selected.png" );
const char* GRID_LAYOUT_IMAGE( DALI_IMAGE_DIR "icon-item-view-layout-grid.png" );
const char* GRID_LAYOUT_IMAGE_SELECTED( DALI_IMAGE_DIR "icon-item-view-layout-grid-selected.png" );
const char* DEPTH_LAYOUT_IMAGE( DALI_IMAGE_DIR "icon-item-view-layout-depth.png" );
const char* DEPTH_LAYOUT_IMAGE_SELECTED( DALI_IMAGE_DIR "icon-item-view-layout-depth-selected.png" );
const char* DELETE_IMAGE( DALI_IMAGE_DIR "icon-delete.png" );
const char* DELETE_IMAGE_SELECTED( DALI_IMAGE_DIR "icon-delete-selected.png" );
const char* REPLACE_IMAGE( DALI_IMAGE_DIR "icon-replace.png" );
const char* REPLACE_IMAGE_SELECTED( DALI_IMAGE_DIR "icon-replace-selected.png" );
const char* INSERT_IMAGE( DALI_IMAGE_DIR "icon-insert.png" );
const char* INSERT_IMAGE_SELECTED( DALI_IMAGE_DIR "icon-insert-selected.png" );
const char* SELECTED_IMAGE( DALI_IMAGE_DIR "item-select-check.png" );
const char* APPLICATION_TITLE( "ItemView" );

const char* SPIRAL_LABEL("Spiral");
const char* GRID_LABEL("Grid");
const char* DEPTH_LABEL("Depth");

const char* ITEM_BORDER_IMAGE_PATH( DALI_IMAGE_DIR "frame-128x128.png" );
const Vector3 ITEM_BORDER_MARGIN_SIZE(24, 24, 0);

// These values depend on the border image
const float ITEM_IMAGE_BORDER_LEFT   = 13.0f;
const float ITEM_IMAGE_BORDER_RIGHT  = 13.0f;
const float ITEM_IMAGE_BORDER_TOP    = 13.0f;
const float ITEM_IMAGE_BORDER_BOTTOM = 13.0f;

const float DEPTH_LAYOUT_ITEM_SIZE_FACTOR_PORTRAIT = 1.0f;
const float DEPTH_LAYOUT_ITEM_SIZE_FACTOR_LANDSCAPE = 0.8f;
const float DEPTH_LAYOUT_COLUMNS = 3.0f;

const float MIN_SWIPE_DISTANCE = 15.0f;
const float MIN_SWIPE_SPEED = 5.0f;

const float SELECTION_BORDER_WIDTH = 3.0f;
const float BUTTON_BORDER = -10.0f;
const float MENU_OPTION_HEIGHT(140.0f);
const float LABEL_TEXT_SIZE_Y = 20.0f;

const Vector3 INITIAL_OFFSCREEN_POSITION( 1000.0f, 0, -1000.0f );

static Vector3 DepthLayoutItemSizeFunctionPortrait( float layoutWidth )
{
  float width = ( layoutWidth / ( DEPTH_LAYOUT_COLUMNS + 1.0f ) ) * DEPTH_LAYOUT_ITEM_SIZE_FACTOR_PORTRAIT;

  // 1x1 aspect ratio
  return Vector3(width, width, width);
}

static Vector3 DepthLayoutItemSizeFunctionLandscape( float layoutWidth )
{
  float width = ( layoutWidth / ( DEPTH_LAYOUT_COLUMNS + 1.0f ) ) * DEPTH_LAYOUT_ITEM_SIZE_FACTOR_LANDSCAPE;

  // 1x1 aspect ratio
  return Vector3(width, width, width);
}

} // unnamed namespace

/**
 * This example shows how to use ItemView UI control.
 * There are three layouts created for ItemView, i.e., Spiral, Depth and Grid.
 * There is one button in the upper-left corner for quitting the application and
 * another button in the upper-right corner for switching between different layouts.
 */
class ItemViewExample : public ConnectionTracker, public ItemFactory
{
public:

  enum Mode
  {
    MODE_NORMAL,
    MODE_REMOVE,
    MODE_REMOVE_MANY,
    MODE_INSERT,
    MODE_INSERT_MANY,
    MODE_REPLACE,
    MODE_REPLACE_MANY,
    MODE_LAST
  };

  /**
   * Constructor
   * @param application class, stored as reference
   */
  ItemViewExample( Application& application )
  : mApplication( application ),
    mMode( MODE_NORMAL ),
    mOrientation( 0 ),
    mCurrentLayout( SPIRAL_LAYOUT ),
    mDurationSeconds( 0.25f )
  {
    // Connect to the Application's Init signal
    mApplication.InitSignal().Connect(this, &ItemViewExample::OnInit);
  }

  /**
   * This method gets called once the main loop of application is up and running
   */
  void OnInit(Application& app)
  {
    Stage stage = Dali::Stage::GetCurrent();
    stage.KeyEventSignal().Connect(this, &ItemViewExample::OnKeyEvent);
    stage.GetRootLayer().SetBehavior(Layer::LAYER_3D);

    Vector2 stageSize = Stage::GetCurrent().GetSize();

    // Create a border image shared by all the item actors
    mBorderImage = ResourceImage::New(ITEM_BORDER_IMAGE_PATH);

    // Creates a default view with a default tool bar.
    // The view is added to the stage.
    Layer contents = DemoHelper::CreateView( mApplication,
                                             mView,
                                             mToolBar,
                                             BACKGROUND_IMAGE,
                                             TOOLBAR_IMAGE,
                                             "" );

    //app.GetWindow().GetOrientation().ChangedSignal().Connect( this, &ItemViewExample::OnOrientationChanged );

    // Create an edit mode button. (left of toolbar)
    Toolkit::PushButton editButton = Toolkit::PushButton::New();
    editButton.SetUnselectedImage( EDIT_IMAGE );
    editButton.SetSelectedImage( EDIT_IMAGE_SELECTED );
    editButton.ClickedSignal().Connect( this, &ItemViewExample::OnModeButtonClicked);
    editButton.SetLeaveRequired( true );
    mToolBar.AddControl( editButton, DemoHelper::DEFAULT_VIEW_STYLE.mToolBarButtonPercentage, Toolkit::Alignment::HorizontalLeft, DemoHelper::DEFAULT_MODE_SWITCH_PADDING  );

    // Create a layout toggle button. (right of toolbar)
    mLayoutButton = Toolkit::PushButton::New();
    mLayoutButton.SetUnselectedImage( SPIRAL_LAYOUT_IMAGE );
    mLayoutButton.SetSelectedImage(SPIRAL_LAYOUT_IMAGE_SELECTED );
    mLayoutButton.ClickedSignal().Connect( this, &ItemViewExample::OnLayoutButtonClicked);
    mLayoutButton.SetLeaveRequired( true );
    mToolBar.AddControl( mLayoutButton, DemoHelper::DEFAULT_VIEW_STYLE.mToolBarButtonPercentage, Toolkit::Alignment::HorizontalRight, DemoHelper::DEFAULT_MODE_SWITCH_PADDING  );

    // Create a delete button (bottom right of screen)
    mDeleteButton = Toolkit::PushButton::New();
    mDeleteButton.SetParentOrigin(ParentOrigin::BOTTOM_RIGHT);
    mDeleteButton.SetAnchorPoint(AnchorPoint::BOTTOM_RIGHT);
    mDeleteButton.SetPosition( BUTTON_BORDER, BUTTON_BORDER );
    mDeleteButton.SetDrawMode( DrawMode::OVERLAY_2D );
    mDeleteButton.SetUnselectedImage( DELETE_IMAGE );
    mDeleteButton.SetSelectedImage( DELETE_IMAGE_SELECTED );
    mDeleteButton.SetBackgroundImage( TOOLBAR_IMAGE );
    mDeleteButton.SetSize( Vector2( stageSize.width * 0.15f, stageSize.width * 0.15f ) );
    mDeleteButton.ClickedSignal().Connect( this, &ItemViewExample::OnDeleteButtonClicked);
    mDeleteButton.SetLeaveRequired( true );
    mDeleteButton.SetVisible( false );
    stage.Add( mDeleteButton );

    // Create an insert button (bottom right of screen)
    mInsertButton = Toolkit::PushButton::New();
    mInsertButton.SetParentOrigin(ParentOrigin::BOTTOM_RIGHT);
    mInsertButton.SetAnchorPoint(AnchorPoint::BOTTOM_RIGHT);
    mInsertButton.SetPosition( BUTTON_BORDER, BUTTON_BORDER );
    mInsertButton.SetDrawMode( DrawMode::OVERLAY_2D );
    mInsertButton.SetUnselectedImage( INSERT_IMAGE );
    mInsertButton.SetSelectedImage( INSERT_IMAGE_SELECTED );
    mInsertButton.SetBackgroundImage( TOOLBAR_IMAGE );
    mInsertButton.SetSize( stageSize.width * 0.15f, stageSize.width * 0.15f );
    mInsertButton.ClickedSignal().Connect( this, &ItemViewExample::OnInsertButtonClicked);
    mInsertButton.SetLeaveRequired( true );
    mInsertButton.SetVisible( false );
    stage.Add( mInsertButton );

    // Create an replace button (bottom right of screen)
    mReplaceButton = Toolkit::PushButton::New();
    mReplaceButton.SetParentOrigin(ParentOrigin::BOTTOM_RIGHT);
    mReplaceButton.SetAnchorPoint(AnchorPoint::BOTTOM_RIGHT);
    mReplaceButton.SetPosition( BUTTON_BORDER, BUTTON_BORDER );
    mReplaceButton.SetDrawMode( DrawMode::OVERLAY_2D );
    mReplaceButton.SetUnselectedImage( REPLACE_IMAGE );
    mReplaceButton.SetSelectedImage( REPLACE_IMAGE_SELECTED );
    mReplaceButton.SetBackgroundImage( TOOLBAR_IMAGE );
    mReplaceButton.SetSize( stageSize.width * 0.15f, stageSize.width * 0.15f );
    mReplaceButton.ClickedSignal().Connect( this, &ItemViewExample::OnReplaceButtonClicked);
    mReplaceButton.SetLeaveRequired( true );
    mReplaceButton.SetVisible( false );
    stage.Add( mReplaceButton );

    // Store one 1x1 white image for multiple items to share for backgrounds:
    mWhiteImage = BufferImage::WHITE();

    // Create the item view actor
    mImageAtlas = CreateImageAtlas();
    mItemView = ItemView::New(*this);
    mItemView.SetParentOrigin(ParentOrigin::CENTER);
    mItemView.SetAnchorPoint(AnchorPoint::CENTER);

    // Display item view on the stage
    stage.Add( mItemView );
    stage.GetRootLayer().SetBehavior( Layer::LAYER_3D );

    // Create the layouts
    mSpiralLayout = DefaultItemLayout::New( DefaultItemLayout::SPIRAL );
    mDepthLayout = DefaultItemLayout::New( DefaultItemLayout::DEPTH );
    mGridLayout = DefaultItemLayout::New( DefaultItemLayout::GRID );

    // Add the layouts to item view
    mItemView.AddLayout(*mSpiralLayout);
    mItemView.AddLayout(*mDepthLayout);
    mItemView.AddLayout(*mGridLayout);

    mItemView.SetMinimumSwipeDistance(MIN_SWIPE_DISTANCE);
    mItemView.SetMinimumSwipeSpeed(MIN_SWIPE_SPEED);

    // Activate the spiral layout
    SetLayout( mCurrentLayout );
    mItemView.SetKeyboardFocusable( true );
    KeyboardFocusManager::Get().PreFocusChangeSignal().Connect( this, &ItemViewExample::OnKeyboardPreFocusChange );

    // Set the title and icon to the current layout
    SetLayoutTitle();
    SetLayoutImage();
  }

  Actor OnKeyboardPreFocusChange( Actor current, Actor proposed, Control::KeyboardFocus::Direction direction )
  {
    if ( !current && !proposed  )
    {
      return mItemView;
    }

    return proposed;
  }

  /**
   * Animate to a different layout
   */
  void ChangeLayout()
  {
    Animation animation = Animation::New( mDurationSeconds );
    animation.FinishedSignal().Connect( this, &ItemViewExample::AnimationFinished );
    animation.AnimateTo( Property( mItemView, Actor::Property::COLOR_ALPHA ), 0.0f );
    animation.Play();
  }

  void AnimationFinished( Animation& )
  {
    SetLayout( mCurrentLayout );

    Animation animation = Animation::New( mDurationSeconds );
    animation.AnimateTo( Property( mItemView, Actor::Property::COLOR_ALPHA ), 1.0f );
    animation.Play();
  }

  /**
   * Switch to a different item view layout
   */
  void SetLayout( int layoutId )
  {
    // Set the new orientation to the layout
    mItemView.GetLayout(layoutId)->SetOrientation(static_cast<ControlOrientation::Type>(mOrientation / 90));

    Vector2 stageSize = Stage::GetCurrent().GetSize();

    if(layoutId == DEPTH_LAYOUT)
    {
      // Set up the depth layout according to the new orientation
      if(Toolkit::IsVertical(mDepthLayout->GetOrientation()))
      {
        mDepthLayout->SetItemSize( DepthLayoutItemSizeFunctionPortrait( stageSize.width ) );
      }
      else
      {
        mDepthLayout->SetItemSize( DepthLayoutItemSizeFunctionLandscape( stageSize.height ) );
      }
    }

    // Enable anchoring for depth layout only
    mItemView.SetAnchoring(layoutId == DEPTH_LAYOUT);

    // Activate the layout
    mItemView.ActivateLayout( layoutId, Vector3(stageSize.x, stageSize.y, stageSize.x), 0.0f );
  }

  /**
   * Orientation changed signal callback
   * @param orientation
   */
  void OnOrientationChanged( Orientation orientation )
  {
    const unsigned int angle = orientation.GetDegrees();

    // If orientation really changed
    if( mOrientation != angle )
    {
      // Remember orientation
      mOrientation = angle;

      SetLayout( mCurrentLayout );
    }
  }

  bool OnLayoutButtonClicked( Toolkit::Button button )
  {
    // Switch to the next layout
    mCurrentLayout = (mCurrentLayout + 1) % mItemView.GetLayoutCount();

    ChangeLayout();

    SetLayoutTitle();
    SetLayoutImage();

    return true;
  }

  bool OnModeButtonClicked( Toolkit::Button button )
  {
    SwitchToNextMode();

    return true;
  }

  void SwitchToNextMode()
  {
    switch( mMode )
    {
      case MODE_REMOVE:
      {
        ExitRemoveMode();
        mMode = MODE_REMOVE_MANY;
        EnterRemoveManyMode();
        break;
      }

      case MODE_REMOVE_MANY:
      {
        ExitRemoveManyMode();
        mMode = MODE_INSERT;
        EnterInsertMode();
        break;
      }

      case MODE_INSERT:
      {
        ExitInsertMode();
        mMode = MODE_INSERT_MANY;
        EnterInsertManyMode();
        break;
      }

      case MODE_INSERT_MANY:
      {
        ExitInsertManyMode();
        mMode = MODE_REPLACE;
        EnterReplaceMode();
        break;
      }

      case MODE_REPLACE:
      {
        ExitReplaceMode();
        mMode = MODE_REPLACE_MANY;
        EnterReplaceManyMode();
        break;
      }

      case MODE_REPLACE_MANY:
      {
        ExitReplaceManyMode();
        mMode = MODE_NORMAL;
        SetLayoutTitle();
        break;
      }

      case MODE_NORMAL:
      default:
      {
        mMode = MODE_REMOVE;
        EnterRemoveMode();
        break;
      }
    }
  }

  void EnterRemoveMode()
  {
    SetTitle("Edit: Remove");

    mTapDetector = TapGestureDetector::New();

    for( unsigned int i = 0u; i < mItemView.GetChildCount(); ++i )
    {
      mTapDetector.Attach(mItemView.GetChildAt(i));
    }

    mTapDetector.DetectedSignal().Connect( this, &ItemViewExample::RemoveOnTap );
  }

  void ExitRemoveMode()
  {
    mTapDetector.Reset();
  }

  void RemoveOnTap( Actor actor, const TapGesture& tap )
  {
    mItemView.RemoveItem( mItemView.GetItemId(actor), 0.5f );
  }

  void EnterRemoveManyMode()
  {
    SetTitle("Edit: Remove Many");

    mDeleteButton.SetVisible( true );

    mTapDetector = TapGestureDetector::New();

    for( unsigned int i = 0u; i < mItemView.GetChildCount(); ++i )
    {
      Actor child = mItemView.GetChildAt( i );
      Actor box = child.FindChildByName( "CheckBox" );

      if( box )
      {
        mTapDetector.Attach( child );
        box.SetVisible( true );
      }
    }

    mTapDetector.DetectedSignal().Connect( this, &ItemViewExample::SelectOnTap );
  }

  void ExitRemoveManyMode()
  {
    for( unsigned int i = 0u; i < mItemView.GetChildCount(); ++i )
    {
      Actor child = mItemView.GetChildAt( i );
      Actor box = child.FindChildByName( "CheckBox" );

      if( box )
      {
        box.SetVisible( false );

        Actor tick = box.FindChildByName( "Tick" );
        if( tick )
        {
          tick.SetVisible( false );
        }
      }
    }

    mTapDetector.Reset();

    mDeleteButton.SetVisible( false );
  }

  void SelectOnTap( Actor actor, const TapGesture& tap )
  {
    Actor tick = actor.FindChildByName( "Tick" );
    if( tick )
    {
      tick.SetVisible( !tick.IsVisible() );
    }
  }

  bool OnDeleteButtonClicked( Toolkit::Button button )
  {
    ItemIdContainer removeList;

    for( unsigned int i = 0u; i < mItemView.GetChildCount(); ++i )
    {
      Actor child = mItemView.GetChildAt( i );
      Actor tick = child.FindChildByName( "Tick" );

      if( tick && tick.IsVisible() )
      {
        removeList.push_back( mItemView.GetItemId(child) );
      }
    }

    if( ! removeList.empty() )
    {
      mItemView.RemoveItems( removeList, 0.5f );
    }

    return true;
  }

  void EnterInsertMode()
  {
    SetTitle("Edit: Insert");

    mTapDetector = TapGestureDetector::New();

    for( unsigned int i = 0u; i < mItemView.GetChildCount(); ++i )
    {
      mTapDetector.Attach( mItemView.GetChildAt(i) );
    }

    mTapDetector.DetectedSignal().Connect( this, &ItemViewExample::InsertOnTap );
  }

  void ExitInsertMode()
  {
    mTapDetector.Reset();
  }

  void InsertOnTap( Actor actor, const TapGesture& tap )
  {
    ItemId id = mItemView.GetItemId( actor );

    Actor newActor = NewItem( rand() );

    mItemView.InsertItem( Item(id,newActor), 0.5f );
  }

  void EnterInsertManyMode()
  {
    SetTitle("Edit: Insert Many");

    mInsertButton.SetVisible( true );

    mTapDetector = TapGestureDetector::New();

    for( unsigned int i = 0u; i < mItemView.GetChildCount(); ++i )
    {
      Actor child = mItemView.GetChildAt( i );
      Actor box = child.FindChildByName( "CheckBox" );

      if( box )
      {
        mTapDetector.Attach( child );
        box.SetVisible( true );
      }
    }

    mTapDetector.DetectedSignal().Connect( this, &ItemViewExample::SelectOnTap );
  }

  void ExitInsertManyMode()
  {
    for( unsigned int i = 0u; i < mItemView.GetChildCount(); ++i )
    {
      Actor child = mItemView.GetChildAt( i );
      Actor box = child.FindChildByName( "CheckBox" );

      if( box )
      {
        box.SetVisible( false );

        Actor tick = box.FindChildByName( "Tick" );
        if( tick )
        {
          tick.SetVisible( false );
        }
      }
    }

    mTapDetector.Reset();

    mInsertButton.SetVisible( false );
  }

  bool OnInsertButtonClicked( Toolkit::Button button )
  {
    ItemContainer insertList;

    for( unsigned int i = 0u; i < mItemView.GetChildCount(); ++i )
    {
      Actor child = mItemView.GetChildAt( i );
      Actor tick = child.FindChildByName( "Tick" );

      if( tick && tick.IsVisible() )
      {
        insertList.push_back( Item( mItemView.GetItemId(child), NewItem(rand()) ) );
      }
    }

    if( ! insertList.empty() )
    {
      mItemView.InsertItems( insertList, 0.5f );
    }

    return true;
  }

  void EnterReplaceMode()
  {
    SetTitle("Edit: Replace");

    mTapDetector = TapGestureDetector::New();

    for( unsigned int i = 0u; i < mItemView.GetChildCount(); ++i )
    {
      mTapDetector.Attach(mItemView.GetChildAt(i));
    }

    mTapDetector.DetectedSignal().Connect( this, &ItemViewExample::ReplaceOnTap );
  }

  void ReplaceOnTap( Actor actor, const TapGesture& tap )
  {
    mItemView.ReplaceItem( Item( mItemView.GetItemId(actor), NewItem(rand()) ), 0.5f );
  }

  void ExitReplaceMode()
  {
    mTapDetector.Reset();
  }

  void EnterReplaceManyMode()
  {
    SetTitle("Edit: Replace Many");

    mReplaceButton.SetVisible( true );

    mTapDetector = TapGestureDetector::New();

    for( unsigned int i = 0u; i < mItemView.GetChildCount(); ++i )
    {
      Actor child = mItemView.GetChildAt( i );
      Actor box = child.FindChildByName( "CheckBox" );

      if( box )
      {
        mTapDetector.Attach( child );
        box.SetVisible( true );
      }
    }

    mTapDetector.DetectedSignal().Connect( this, &ItemViewExample::SelectOnTap );
  }

  void ExitReplaceManyMode()
  {
    for( unsigned int i = 0u; i < mItemView.GetChildCount(); ++i )
    {
      Actor child = mItemView.GetChildAt( i );
      Actor box = child.FindChildByName( "CheckBox" );

      if( box )
      {
        box.SetVisible( false );

        Actor tick = box.FindChildByName( "Tick" );
        if( tick )
        {
          tick.SetVisible( false );
        }
      }
    }

    mTapDetector.Reset();

    mReplaceButton.SetVisible( false );
  }

  bool OnReplaceButtonClicked( Toolkit::Button button )
  {
    ItemContainer replaceList;

    for( unsigned int i = 0u; i < mItemView.GetChildCount(); ++i )
    {
      Actor child = mItemView.GetChildAt( i );
      Actor tick = child.FindChildByName( "Tick" );

      if( tick && tick.IsVisible() )
      {
        replaceList.push_back( Item( mItemView.GetItemId(child), NewItem(rand()) ) );
      }
    }

    if( ! replaceList.empty() )
    {
      mItemView.ReplaceItems( replaceList, 0.5f );
    }

    return true;
  }

  void SetLayoutTitle()
  {
    if( MODE_NORMAL == mMode )
    {
      std::stringstream ss(APPLICATION_TITLE);
      switch(mCurrentLayout)
      {
      case SPIRAL_LAYOUT:
        ss << APPLICATION_TITLE << ": " << SPIRAL_LABEL;
        break;
      case GRID_LAYOUT:
        ss << APPLICATION_TITLE << ": " << GRID_LABEL;
        break;
      case DEPTH_LAYOUT:
        ss << APPLICATION_TITLE << ": " << DEPTH_LABEL;
        break;
      default:
        break;
      }
      SetTitle(ss.str());
    }
  }

  void SetLayoutImage()
  {
    if( mLayoutButton )
    {
      switch( mCurrentLayout )
      {
        case SPIRAL_LAYOUT:
        {
          mLayoutButton.SetUnselectedImage( SPIRAL_LAYOUT_IMAGE );
          mLayoutButton.SetSelectedImage( SPIRAL_LAYOUT_IMAGE_SELECTED );
          break;
        }

        case GRID_LAYOUT:
        {
          mLayoutButton.SetUnselectedImage( GRID_LAYOUT_IMAGE );
          mLayoutButton.SetSelectedImage( GRID_LAYOUT_IMAGE_SELECTED );
          break;
        }

        case DEPTH_LAYOUT:
        {
          mLayoutButton.SetUnselectedImage( DEPTH_LAYOUT_IMAGE );
          mLayoutButton.SetSelectedImage( DEPTH_LAYOUT_IMAGE_SELECTED );
          break;
        }

        default:
          break;
      }
    }
  }

public: // From ItemFactory

  /**
   * Query the number of items available from the factory.
   * The maximum available item has an ID of GetNumberOfItems() - 1.
   */
  virtual unsigned int GetNumberOfItems()
  {
    return NUM_IMAGES * 10;
  }

  /**
   * Create an Actor to represent a visible item.
   * @param itemId
   * @return the created actor.
   */
  virtual Actor NewItem(unsigned int itemId)
  {
    // Create an image actor for this item
    unsigned int imageId = itemId % NUM_IMAGES;
    ImageActor::PixelArea pixelArea( (imageId%NUM_IMAGE_PER_ROW_IN_ATLAS)*IMAGE_WIDTH,
                                     (imageId/NUM_IMAGE_PER_ROW_IN_ATLAS)*IMAGE_HEIGHT,
                                      IMAGE_WIDTH,
                                      IMAGE_HEIGHT );
    Actor actor = ImageActor::New(mImageAtlas, pixelArea);
    actor.SetPosition( INITIAL_OFFSCREEN_POSITION );

    // Add a border image child actor
    ImageActor borderActor = ImageActor::New(mBorderImage);
    borderActor.SetParentOrigin( ParentOrigin::CENTER );
    borderActor.SetAnchorPoint( AnchorPoint::CENTER );
    borderActor.SetPosition( 0.f, 0.f, 1.f );
    borderActor.SetStyle( ImageActor::STYLE_NINE_PATCH );
    borderActor.SetNinePatchBorder( Vector4( ITEM_IMAGE_BORDER_LEFT, ITEM_IMAGE_BORDER_TOP, ITEM_IMAGE_BORDER_RIGHT, ITEM_IMAGE_BORDER_BOTTOM ) );
    borderActor.SetColorMode( USE_OWN_MULTIPLY_PARENT_COLOR ); // darken with parent image-actor
    borderActor.SetResizePolicy( ResizePolicy::SIZE_FIXED_OFFSET_FROM_PARENT, Dimension::ALL_DIMENSIONS );
    borderActor.SetSizeModeFactor( ITEM_BORDER_MARGIN_SIZE );
    actor.Add(borderActor);
    actor.SetKeyboardFocusable( true );

    Vector3 spiralItemSize;
    static_cast<ItemLayout&>(*mSpiralLayout).GetItemSize( 0u, Vector3( Stage::GetCurrent().GetSize() ), spiralItemSize );

    // Add a checkbox child actor; invisible until edit-mode is enabled

    ImageActor checkbox = ImageActor::New( mWhiteImage );
    checkbox.SetName( "CheckBox" );
    checkbox.SetColor( Vector4(0.0f,0.0f,0.0f,0.6f) );
    checkbox.SetParentOrigin( ParentOrigin::TOP_RIGHT );
    checkbox.SetAnchorPoint( AnchorPoint::TOP_RIGHT );
    checkbox.SetSize( spiralItemSize.width * 0.2f, spiralItemSize.width * 0.2f );
    checkbox.SetPosition( -SELECTION_BORDER_WIDTH, SELECTION_BORDER_WIDTH );
    checkbox.SetZ( 1.0f );
    SetCullFace(checkbox, Dali::CullBack);
    checkbox.SetSortModifier( 150.0f );
    if( MODE_REMOVE_MANY  != mMode &&
        MODE_INSERT_MANY  != mMode &&
        MODE_REPLACE_MANY != mMode )
    {
      checkbox.SetVisible( false );
    }
    actor.Add( checkbox );

    ImageActor tick = ImageActor::New( ResourceImage::New(SELECTED_IMAGE) );
    tick.SetColorMode( USE_OWN_COLOR );
    tick.SetName( "Tick" );
    tick.SetParentOrigin( ParentOrigin::TOP_RIGHT );
    tick.SetAnchorPoint( AnchorPoint::TOP_RIGHT );
    tick.SetSize( spiralItemSize.width * 0.2f, spiralItemSize.width * 0.2f );
    tick.SetZ( 1.0f );
    tick.SetSortModifier( 150.0f );
    tick.SetVisible( false );
    SetCullFace(tick, Dali::CullBack);
    checkbox.Add( tick );

    // Connect new items for various editing modes
    if( mTapDetector )
    {
      mTapDetector.Attach( actor );
    }

    return actor;
  }

private:

  /**
   * Create an Atlas to tile the images inside.
   */
  Atlas CreateImageAtlas()
  {
    const unsigned int atlas_width = IMAGE_WIDTH*NUM_IMAGE_PER_ROW_IN_ATLAS;
    const unsigned int atlas_height = IMAGE_HEIGHT*ceil( static_cast<float>(NUM_IMAGES)/ static_cast<float>(NUM_IMAGE_PER_ROW_IN_ATLAS));
    Atlas atlas = Atlas::New(atlas_width, atlas_height, Pixel::RGB888);

    for( unsigned int i = 0; i < NUM_IMAGES; i++ )
    {
      atlas.Upload( IMAGE_PATHS[i], (i%NUM_IMAGE_PER_ROW_IN_ATLAS)*IMAGE_WIDTH, (i/NUM_IMAGE_PER_ROW_IN_ATLAS)*IMAGE_HEIGHT );
    }

    return atlas;
  }

  /**
   * Sets/Updates the title of the View
   * @param[in] title The new title for the view.
   */
  void SetTitle(const std::string& title)
  {
    if(!mTitleActor)
    {
      mTitleActor = DemoHelper::CreateToolBarLabel( "" );
      // Add title to the tool bar.
      mToolBar.AddControl( mTitleActor, DemoHelper::DEFAULT_VIEW_STYLE.mToolBarTitlePercentage, Alignment::HorizontalCenter );
    }

    mTitleActor.SetProperty( TextLabel::Property::TEXT, title );
  }

  /**
   * Main key event handler
   */
  void OnKeyEvent(const KeyEvent& event)
  {
    if(event.state == KeyEvent::Down)
    {
      if( IsKey( event, DALI_KEY_ESCAPE) || IsKey( event, DALI_KEY_BACK ) )
      {
        mApplication.Quit();
      }
    }
  }

private:

  Application& mApplication;
  Mode mMode;

  Toolkit::Control mView;
  unsigned int mOrientation;

  Toolkit::ToolBar mToolBar;
  TextLabel mTitleActor;             ///< The Toolbar's Title.

  ItemView mItemView;
  Image mBorderImage;
  Atlas mImageAtlas;
  unsigned int mCurrentLayout;
  float mDurationSeconds;

  ItemLayoutPtr mSpiralLayout;
  ItemLayoutPtr mDepthLayout;
  ItemLayoutPtr mGridLayout;

  TapGestureDetector mTapDetector;
  Toolkit::PushButton mLayoutButton;
  Toolkit::PushButton mDeleteButton;
  Toolkit::PushButton mInsertButton;
  Toolkit::PushButton mReplaceButton;

  BufferImage mWhiteImage;
};

void RunTest(Application& app)
{
  ItemViewExample test(app);

  app.MainLoop();
}

int main(int argc, char **argv)
{
  Application app = Application::New(&argc, &argv, DALI_DEMO_THEME_PATH);

  RunTest(app);

  return 0;
}
