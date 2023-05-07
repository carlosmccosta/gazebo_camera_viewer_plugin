#include <gazebo_camera_viewer_plugin/gazebo_camera_viewer_plugin.h>
#include <functional>
#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>


namespace gazebo_camera_viewer_plugin {

GazeboCameraViewerPlugin::~GazeboCameraViewerPlugin()
{
	newFrameConnection.reset();
	parentSensor.reset();
	camera.reset();
}


void GazeboCameraViewerPlugin::Load(gazebo::sensors::SensorPtr _sensor, sdf::ElementPtr _sdf)
{
	if (!_sensor)
		gzerr << "Invalid sensor pointer.\n";

	parentSensor = std::dynamic_pointer_cast<gazebo::sensors::CameraSensor>(_sensor);

	if (!parentSensor)
	{
		gzerr << "GazeboCameraViewerPlugin requires a CameraSensor.\n";
		return;
	}

	if (_sdf->HasElement("windowName"))
		windowName = _sdf->GetElement("windowName")->Get<std::string>();

	cv::namedWindow(windowName, cv::WINDOW_NORMAL);

	bool fullscreen = _sdf->HasElement("fullscreen") && _sdf->GetElement("fullscreen")->Get<bool>();
	if (fullscreen)
	{
		cv::setWindowProperty(windowName, cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
	}
	else if (_sdf->HasElement("windowWidth") && _sdf->HasElement("windowHeight"))
	{
		int width = _sdf->GetElement("windowWidth")->Get<int>();
		int height = _sdf->GetElement("windowHeight")->Get<int>();
		if (width > 0 && height > 0)
			cv::resizeWindow(windowName, width, height);
	}

	int windowHorizontalOffset = 0, windowVerticalOffset = 0;

	if (_sdf->HasElement("windowHorizontalOffset"))
		windowHorizontalOffset = _sdf->GetElement("windowHorizontalOffset")->Get<int>();

	if (_sdf->HasElement("windowHorizontalOffset"))
		windowVerticalOffset = _sdf->GetElement("windowVerticalOffset")->Get<int>();

	cv::moveWindow(windowName, windowHorizontalOffset, windowVerticalOffset);

	if (_sdf->HasElement("cvWaitKeyMs"))
		cvWaitKeyMs = _sdf->GetElement("cvWaitKeyMs")->Get<int>();

	camera = parentSensor->Camera();

	newFrameConnection = camera->ConnectNewImageFrame(
			std::bind(&GazeboCameraViewerPlugin::OnNewImageFrame, this,
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
				std::placeholders::_4, std::placeholders::_5));

	parentSensor->SetActive(true);
}


void GazeboCameraViewerPlugin::OnNewImageFrame(const unsigned char *_image,
				unsigned int _width, unsigned int _height,
				unsigned int _depth, const std::string& _format)
{
	double time = ((double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) * 1e-3;
	std::stringstream ss;
	ss << "Received frame [" << frameNumber++
		<< "] at time [" << std::fixed << time
		<< "] || [ width: " << _width
		<< " | height: " << _height
		<< " | depth: " << _depth
		<< " | format: " << _format << " ]";
	dbgMsg(ss.str());

	if (lastSecond == 0)
	{
		lastSecond = (size_t)time;
		numberOfFramesInLastSecond = 1;
	}
	else
	{
		if (lastSecond == (size_t)time)
			++numberOfFramesInLastSecond;
		else
		{
			std::stringstream ss;
			ss << "Number of frames received in last second (FPS): " << numberOfFramesInLastSecond;
			logMsg(ss.str());
			lastSecond = (size_t)time;
			numberOfFramesInLastSecond = 1;
		}
	}

	if (_depth == 1 || _depth == 3)
	{
		image = cv::Mat((int)_height, (int)_width, (_depth == 1 ? CV_8UC1 : CV_8UC3), (void*)_image);
		cv::imshow(windowName, image);
		cv::waitKey(cvWaitKeyMs);
	}
	else
	{
		gzerr << "Discarded image since only images with 1 or 3 bytes of data per pixel are supported.\n";
	}
}


void GazeboCameraViewerPlugin::logMsg(const std::string& _msg)
{
	gzlog << "[GazeboCameraViewerPlugin][windowName:" << windowName << "] " << _msg << std::endl;
}


void GazeboCameraViewerPlugin::dbgMsg(const std::string& _msg)
{
	gzdbg << "[GazeboCameraViewerPlugin][windowName:" << windowName << "] " << _msg << std::endl;
}

}
