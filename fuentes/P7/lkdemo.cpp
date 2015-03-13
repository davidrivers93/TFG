#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string>
#include <iostream>
#include <ctype.h>
#include <time.h>
using namespace cv;
using namespace std;

void help()
{
    // print a welcome message, and the OpenCV version
    cout << "\nThis is a demo of Lukas-Kanade optical flow lkdemo(),\n"
    		"Using OpenCV version %s\n" << CV_VERSION << "\n"
    		<< endl;

    cout << "\nHot keys: \n"
            "\tESC - quit the program\n"
            "\tr - auto-initialize tracking\n"
            "\tc - delete all the points\n"
            "\tn - switch the \"night\" mode on/off\n"
            "To add/remove a feature point click it\n" << endl;
}

Point2f pt;
bool addRemovePt = false;

void onMouse( int event, int x, int y, int /*flags*/, void* /*param*/ )
{
    if( event == CV_EVENT_LBUTTONDOWN )
    {
        pt = Point2f((float)x,(float)y);
        addRemovePt = true;
    }
}

int main( int argc, char** argv )
{
    VideoCapture cap;
    TermCriteria termcrit(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03);
    Size subPixWinSize(10,10), winSize(31,31);
    
    const int MAX_COUNT = 500;
    bool needToInit = true;
    bool nightMode = false;
    
	clock_t t_total = 0;

	double capt_width = 320.0;
	double capt_height = 240.0;

    if( argc == 1 || (argc == 2 && strlen(argv[1]) == 1 && isdigit(argv[1][0]))) {
        cap.open(argc == 2 ? argv[1][0] - '0' : 0);
		cap.set(CV_CAP_PROP_FRAME_WIDTH , capt_width);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT, capt_height);
    }
    else if( argc == 2 )
        cap.open(argv[1]);

    if( !cap.isOpened() )
    {
        cout << "Could not initialize capturing...\n";
        return 0;
    }

    help();

    namedWindow( "LK Demo", 1 );
    setMouseCallback( "LK Demo", onMouse, 0 );

    Mat gray, prevGray, image;
    vector<Point2f> points[2];
    int k=0;
    int nptos_total = 0;
    for(;;k++)
    {
        Mat frame;
        cap >> frame;
        if( frame.empty() )
            break;

        frame.copyTo(image);
        cvtColor(image, gray, CV_BGR2GRAY); 

        if( nightMode )
            image = Scalar::all(0);

        if( needToInit )
        {
            // automatic initialization
            goodFeaturesToTrack(gray, points[1], int (MAX_COUNT), double(0.01), int(5), Mat(), int(3), false, double(0.04));
            cornerSubPix(gray, points[1], subPixWinSize, Size(-1,-1), termcrit);
            addRemovePt = false;
        }
        else if( !points[0].empty() )
        {
            vector<uchar> status;
            vector<float> err;
            if(prevGray.empty())
                gray.copyTo(prevGray);
        	clock_t t1 = clock();
            calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize,
                                 3, termcrit, 0, 0.001);
            clock_t t2 = clock();

            t_total += (t2-t1);
            nptos_total += points[0].size();

            size_t i, k;
            for( i = k = 0; i < points[1].size(); i++ )
            {
                if( addRemovePt )
                {
                    if( norm(pt - points[1][i]) <= 5 )
                    {
                        addRemovePt = false;
                        continue;
                    }
                }

                if( !status[i] )
                    continue;

                points[1][k++] = points[1][i];
                circle( image, points[1][i], 3, Scalar(0,255,0), -1, 8);
            }
            points[1].resize(k);
        }


		if( k%100 ==99){
			float ips = float(k) / (t_total / float(CLOCKS_PER_SEC));
			std::cout << "IPS = " << ips << "images/sec\n";
			std::cout << "Tproc = " << 1000/ips << "msec/img\n";
			std::cout << "Tproc = " << 1000000.0/nptos_total << "usec/point\n";
			std::cout << "Nptos/frame = " << nptos_total/ k << "\n\n";
		}


        if( addRemovePt && points[1].size() < (size_t)MAX_COUNT )
        {
            vector<Point2f> tmp;
            tmp.push_back(pt);
   //         cornerSubPix( gray, tmp, winSize, cvSize(-1,-1), termcrit);
            points[1].push_back(tmp[0]);
            addRemovePt = false;
        }

        needToInit = false;
        imshow("LK Demo", image);

        char c = (char)waitKey(10);
        if( c == 27 )
            break;
        switch( c )
        {
        case 'r':
            needToInit = true;
            break;
        case 'c':
            points[1].clear();
            points[0].clear();
            break;
        case 'n':
            nightMode = !nightMode;
            break;
        default:
            break;
        }
        
        std::swap(points[1], points[0]);
        swap(prevGray, gray);
    }

    return 0;
}
