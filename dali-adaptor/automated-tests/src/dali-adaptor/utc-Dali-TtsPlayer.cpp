/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
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

#include <dali/dali.h>
#include <dali-test-suite-utils.h>

using namespace Dali;

void utc_dali_ttsplayer_startup(void)
{
  test_return_value = TET_UNDEF;
}

void utc_dali_ttsplayer_cleanup(void)
{
  test_return_value = TET_PASS;
}

namespace
{

} // unnamed namespace

int UtcDaliTtsPlayerConstructorP(void)
{
  Dali::TtsPlayer player;
  DALI_TEST_CHECK( !player );
  END_TEST;
}

int UtcDaliTtsPlayerCopyConstructorP(void)
{
  Dali::TtsPlayer player;
  Dali::TtsPlayer copy( player );
  DALI_TEST_CHECK( copy == player );

  END_TEST;
}

int UtcDaliTtsPlayerAssignmentOperatorP(void)
{
  Dali::TtsPlayer player;
  Dali::TtsPlayer copy;
  DALI_TEST_CHECK( ! copy );
  copy = player;
  DALI_TEST_CHECK( copy == player );

  END_TEST;
}

int UtcDaliTtsPlayerDestructorP(void)
{
  Dali::TtsPlayer* player = new Dali::TtsPlayer();
  delete player;

  DALI_TEST_CHECK( true );
  END_TEST;
}

int UtcDaliTtsPlayerConstructorFromInternalPointerN(void)
{
  Internal::Adaptor::TtsPlayer* internalPlayer = NULL;
  Dali::TtsPlayer player(internalPlayer);
  DALI_TEST_CHECK( !player ); // Should not reach here!

  END_TEST;
}

int UtcDaliTtsPlayerGetP(void)
{
  Dali::TtsPlayer player = Dali::TtsPlayer::Get();
  DALI_TEST_CHECK( !player );
  END_TEST;
}

int UtcDaliTtsPlayerPlayN(void)
{
  Dali::TtsPlayer player = Dali::TtsPlayer::Get();

  try
  {
    player.Play("text");
    DALI_TEST_CHECK( false ); // Should not reach here!
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliTtsPlayerStopN(void)
{
  Dali::TtsPlayer player = Dali::TtsPlayer::Get();

  try
  {
    player.Stop();
    DALI_TEST_CHECK( false ); // Should not reach here!
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliTtsPlayerPauseN(void)
{
  Dali::TtsPlayer player = Dali::TtsPlayer::Get();

  try
  {
    player.Pause();
    DALI_TEST_CHECK( false ); // Should not reach here!
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliTtsPlayerResumeN(void)
{
  Dali::TtsPlayer player = Dali::TtsPlayer::Get();

  try
  {
    player.Resume();
    DALI_TEST_CHECK( false ); // Should not reach here!
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

int UtcDaliTtsPlayerGetStateN(void)
{
  Dali::TtsPlayer player = Dali::TtsPlayer::Get();

  try
  {
    Dali::TtsPlayer::State state = player.GetState();
    tet_printf( "Error: TtsPlayer state = %d, expected exception\n", (unsigned int)state );
    DALI_TEST_CHECK( false ); // Should not reach here!
  }
  catch( ... )
  {
    DALI_TEST_CHECK( true );
  }

  END_TEST;
}

