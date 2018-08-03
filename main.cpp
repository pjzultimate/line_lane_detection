#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;
using namespace cv;
void detection_lane(Mat& image)
{

    Mat image1=image.clone();
    Mat image2,image3;
    cvtColor(image1,image1,CV_RGB2GRAY);
    GaussianBlur(image1,image2,Size(3,3),0);
    //imshow("test1",image2);
    Canny(image2,image3,10,180);
    //imshow("test1",image3);
    //预处理设置感兴趣区域，根据轮廓模式找到我们想要的ROI区域
    Mat mask=Mat::zeros(image3.size(),CV_8UC1);
    Mat mask1;
    vector<vector<Point>> contour;
    vector<Point>point;
    point.push_back(Point(440,335));
    point.push_back(Point(540,335));
    point.push_back(Point(900,530));
    point.push_back(Point(100,530));
    contour.push_back(point);
    drawContours(mask,contour,0,Scalar::all(255),-1);
    image3.copyTo(mask1,mask);
    imshow("ROI",mask1);
    //对感兴趣区域处理
    vector<Vec4i>lines,lines1;
    HoughLinesP(mask1,lines1,1,CV_PI/180,25,20,10);//提取感兴趣区域的直线;
    //求得直线的斜率和平均斜率，找到最接近平均斜率的真实直线，并在图像中延长直线
    for(size_t i=0;i<lines1.size();i++)//过滤掉角度过大或者过小的直线
    {
        double kk = (lines1[i][3]-lines1[i][1])/(lines1[i][2]-lines1[i][0]+0.0000001);
        double  kkk=abs(kk);
        if(0.5<kkk && kkk<0.8)
        {
            lines.push_back(lines1[i]);
        }
    }
    double k1,k2;
    int n1=0,n2=0;
    vector<double> temp;
    for(size_t i=0;i<lines.size();i++)
    {
        double k = (lines[i][3]-lines[i][1])/(lines[i][2]-lines[i][0]+0.0000001);
        temp.push_back(k);
        if(k<0)
        {
            k1+=k;
            n1++;

        } else{
            k2+=k;
            n2++;
        }
        //cout<<Point(lines[i][0],lines[i][1])<<";"<<Point(lines[i][2],lines[i][3])<<";"<<k<<endl;
        //line(image,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(255,0,0),2,LINE_8);
    }
    k1=k1/n1;
    k2=k2/n2;
    double k3,k4;
    double a1=0.05;
    double  a2=0.05;

    int x1=0,x2=0;
    for(size_t i=0;i<temp.size();i++)
    {


        if(temp[i]<0)
        {


            //cout<<"a1:"<<temp[i]-k1<<";"<<temp[i]<<";"<<k1<<endl;
            if(a1>abs((temp[i]-k1)))
            {

                a1=abs((temp[i]-k1));
                k3=temp[i];
                x1=i;

            }

        } else
        {
            //cout<<"a2:"<<temp[i]-k2<<";"<<temp[i]<<";"<<k2<<endl;
            if(a2>abs((temp[i]-k2)))
            {
                a2=abs((temp[i]-k1));
                k4=temp[i];
                x2=i;
            }
        }

    }
    //cout<<x1<<";"<<x2<<endl;
    //cout<<":"<<k3<<";"<<k4<<endl;
    double b1=lines[x1][1]-k3*lines[x1][0];
    double b2=lines[x2][1]-k4*lines[x2][0];
    //cout<<b1<<";"<<b2<<endl;
    line(image,Point((335-b1)/k3,340),Point((540-b1)/k3,540),Scalar(255,255,0),5,LINE_8);
    line(image,Point((335-b2)/k4,340),Point((540-b2)/k4,540),Scalar(255,255,0),5,LINE_8);
    imshow("test2",image);
    cvWaitKey(100);//调整帧率
}

int main() {
    //Mat image=imread("/home/pjz/Lane_detection/test_images/solidYellowCurve.jpg");
    //Mat image=imread("/home/pjz/Lane_detection/test_images/solidWhiteRight.jpg");
    //imshow("test",image);
    //VideoCapture cap("/home/pjz/Lane_detection/test_videos/solidYellowLeft.mp4");
    VideoCapture cap("/home/pjz/Lane_detection/test_videos/solidWhiteRight.mp4");
    if(cap.isOpened())
    {
        cout<<"open success"<<endl;
    }
    Mat frame;
    while(cap.isOpened())
    {
     cap >>frame;
     detection_lane(frame);

    }
    return 0;
}