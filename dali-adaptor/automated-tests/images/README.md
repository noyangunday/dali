% Images for Dali Adaptor Automated Testing

Images used in automated tests for the dali-adaptor project.
These images include broken files to test error handling as well as valid
images.

Truncated files are an interesting case and are a plausible consequence of IO
errors such as network disconnection during downloading of an image by an
application.
Some image codecs can sometimes deliver a partial image for such a truncated
file.
It is thus a policy decision for us whether to return an image like this to the
application, to show some junk to the user, or suppress it as a failed load,
and these tests should demonstrate that that policy is applied for all relevant
formats.


