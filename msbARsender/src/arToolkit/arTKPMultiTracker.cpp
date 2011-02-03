#include "arTKPMultiTracker.h"

#include "ARToolKitPlus/TrackerMultiMarkerImpl.h"

ARToolKitPlus::TrackerMultiMarker *tracker;

arTKPMultiTracker::arTKPMultiTracker()
{
    idNumb.loadFont("frabk.ttf", 24);
    memset(armatrix,0,16*sizeof(float));
}

arTKPMultiTracker::~arTKPMultiTracker()
{
    delete tracker;
}


bool arTKPMultiTracker::setup(int width, int height, bool useBCH)
{
    tracker = new ARToolKitPlus::TrackerMultiMarkerImpl<6,6,6, 10, 10>(width,height);

	const char* description = tracker->getDescription();
	printf("ARToolKitPlus compile-time information:\n%s\n\n", description);

	tracker->setPixelFormat(ARToolKitPlus::PIXEL_FORMAT_LUM);

    if( !tracker->init( "data/LogitechPro4000.dat", "data/markerboard_480-499_small.cfg", 1.0f, 1000.0f) )            // load std. ARToolKit camera file
	{
		printf("ERROR: init() failed\n");
		delete tracker;
		return false;
	}

    // define size of the marker
    //tracker->setPatternWidth(80); // I'm not sure how to define the size with multimarkers since it doesnt seem to have this option.

	// the marker in the BCH test image has a thin border...
    tracker->setBorderWidth(useBCH ? 0.125f : 0.250f);

    // set a threshold. alternatively we could also activate automatic thresholding
    //tracker->setThreshold(20);
	tracker->activateAutoThreshold(true);
    // let's use lookup-table undistortion for high-speed
    // note: LUT only works with images up to 1024x1024
    tracker->setUndistortionMode(ARToolKitPlus::UNDIST_LUT);

    // RPP is more robust than ARToolKit's standard pose estimator
    tracker->setPoseEstimator(ARToolKitPlus::POSE_ESTIMATOR_RPP);

    // switch to simple ID based markers
    // use the tool in tools/IdPatGen to generate markers
    tracker->setMarkerMode(useBCH ? ARToolKitPlus::MARKER_ID_BCH : ARToolKitPlus::MARKER_ID_SIMPLE);

    return true;

}

void arTKPMultiTracker::update( unsigned char * pixels)
{
    //find the marker and get back the confidence
    int markerId = tracker->calc(pixels);
}

void arTKPMultiTracker::draw()
{
    //this is where we use the calculated matrix from ARToolkitPlus to put
	//in our graphics at the location and orientation of the marker.
	//- the matrix has the 0,0 point as the center of the marker.

	int numDetected = tracker->getNumDetectedMarkers();
	cout<<"numberofmarkers:"<<numDetected<<endl;


	glViewport(0, 0, 640, 480 );
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf(tracker->getProjectionMatrix());

	for(int i=0; i<numDetected; i++)
	{

		ARToolKitPlus::ARMarkerInfo marker = tracker->getDetectedMarker(i);

		float m34[ 3 ][ 4 ];
		float c[ 2 ] = { 0.0f, 0.0f };
		float w = 40.0f;


		tracker->rppGetTransMat( &marker, c, w, m34 );
//this is some crazy matrix transformative stuff. I think initially it came out of one of the arToolkit functions.... but i got it from here: http://chihara.naist.jp/people/STAFF/imura/computer/OpenGL/artp/disp_content
        //this turns a 3*4 matrix into an OpenGL 4*4 matrix
		float m[ 16 ];
		for ( int i = 0; i < 3; i++ ) {
			for ( int j = 0; j < 4; j++ ) {
				m[ j * 4 + i ] = m34[ i ][ j ];
			}
		}
		for ( int j = 0; j < 3; j++ ) {
			m[ j * 4 + 3 ] = 0.0f;
		}
		m[ 3 * 4 + 3 ] = 1.0f;

        // copy
        for ( int j = 0; j < 16; j++ )
            armatrix[j] = m[j];


		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		//glScalef(0.01,0.01,0.01);
		glMultMatrixf( m );
        //glMultMatrixf( mScale);

        glGetFloatv(GL_MODELVIEW_MATRIX,(GLfloat*)&armatrix);
		glPushMatrix();
		//glRotatef(-90, 0, 0, 1);
		ofSetColor(255 , 255, 255 );
		//colorPixels.draw(-30,-20,60,40);
		glPopMatrix();

		glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		glRotatef(-90, 0, 1, 0);
		ofSetColor(255 , 0, 0 );
		idNumb.drawString(ofToString(marker.id), -idNumb.stringWidth(ofToString(marker.id))/2,0);
		glPopMatrix();

	}
}
