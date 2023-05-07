#pragma once

#include <gazebo/common/Plugin.hh>
#include <gazebo/sensors/CameraSensor.hh>
#include <gazebo/rendering/Camera.hh>
#include <gazebo/util/system.hh>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


namespace gazebo_camera_viewer_plugin {

class GZ_PLUGIN_VISIBLE GazeboCameraViewerPlugin : public gazebo::SensorPlugin {
	public:
		GazeboCameraViewerPlugin() :
			windowName("camera_viewer"),
			resizeImageWidth(0),
			resizeImageHeight(0),
			resizeImageInterpolation(1),
			cvWaitKeyMs(1),
			frameNumber(0),
			numberOfFramesInLastSecond(0),
			lastSecond(0)
			{}

		virtual ~GazeboCameraViewerPlugin();

		void Load(gazebo::sensors::SensorPtr _sensor, sdf::ElementPtr _sdf) override;
		virtual void OnNewImageFrame(const unsigned char *_image,
						unsigned int _width, unsigned int _height,
						unsigned int _depth, const std::string &_format);

	protected:
		gazebo::sensors::CameraSensorPtr parentSensor;
		gazebo::rendering::CameraPtr camera;
		gazebo::event::ConnectionPtr newFrameConnection;

		cv::Mat image;
		std::string windowName;
		int resizeImageWidth;
		int resizeImageHeight;
		int resizeImageInterpolation;
		int cvWaitKeyMs;

		size_t frameNumber;
		size_t numberOfFramesInLastSecond;
		size_t lastSecond;

		void logMsg(const std::string& _msg);
		void dbgMsg(const std::string& _msg);
};

GZ_REGISTER_SENSOR_PLUGIN(GazeboCameraViewerPlugin)
}
