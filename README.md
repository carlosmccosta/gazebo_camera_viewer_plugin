# gazebo_camera_viewer_plugin

[Gazebo camera sensor plugin](https://classic.gazebosim.org/tutorials?tut=ros_gzplugins) that displays rendered images in full screen or in a window with specific size and position using OpenCV HighGUI.


## Compilation

Compile with cmake and make or within a ROS [catkin-tools](https://catkin-tools.readthedocs.io/) workspace and then add to `.bashrc` the path of the folder where is the plugin `libgazebo_camera_viewer_plugin.so` file.

For example:
```bash
export GAZEBO_PLUGIN_PATH=${GAZEBO_PLUGIN_PATH}:${HOME}/catkin_ws/devel/lib
```


## SDF

Add within a camera sensor the plugin configuration.

```xml
<sensor name='camera_sensor' type='camera'>
    <camera name='camera'>
        <horizontal_fov>1.15</horizontal_fov>
        <image>
            <width>3840</width>
            <height>2160</height>
            <format>B8G8R8</format>
        </image>
        <clip>
            <near>0.01</near>
            <far>3.0</far>
        </clip>
        <noise>
            <type>gaussian</type>
            <mean>0</mean>
            <stddev>0</stddev>
        </noise>
        <lens>
            <intrinsics>
                <fx>5130.4143818257726</fx>
                <fy>5121.1480617483994</fy>
                <cx>1932.4876351658850</cx>
                <cy>1070.2891097604868</cy>
                <s>0.0</s>
            </intrinsics>
        </lens>
        <distortion>
            <k1>0.034193494449851314</k1>
            <k2>0.0063650022546074920</k2>
            <k3>-0.087152376362862452</k3>
            <p1>0.00034912938809033558</p1>
            <p2>-0.0012454893466176376</p2>
        </distortion>
    </camera>
    
    <plugin name='camera_viewer' filename='libgazebo_camera_viewer_plugin.so'>
        <windowName>camera_viewer</windowName>
        <fullscreen>true</fullscreen>
        <windowWidth>0</windowWidth>
        <windowHeight>0</windowHeight>
        <windowHorizontalOffset>0</windowHorizontalOffset>
        <windowVerticalOffset>0</windowVerticalOffset>
        <cvWaitKeyMs>1</cvWaitKeyMs>
        <resizeImageWidth>0</resizeImageWidth>
        <resizeImageHeight>0</resizeImageHeight>
        <resizeImageInterpolation>1</resizeImageInterpolation>
    </plugin>
</sensor>
```

If `windowWidth > 0 && windowHeight > 0`, then `cv::resizeWindow` will be called.

if `resizeImageWidth > 0 && resizeImageHeight > 0 && resizeImageInterpolation >= 0 && (image.cols != resizeImageWidth || image.rows != resizeImageHeight))`, then `cv::resize()` will be called. 

The `resizeImageInterpolation` parameter can be one of the following integers:
- INTER_NEAREST      = 0
- INTER_LINEAR       = 1
- INTER_CUBIC        = 2
- INTER_AREA         = 3
- INTER_LANCZOS4     = 4
- INTER_LINEAR_EXACT = 5
- INTER_MAX          = 7
- WARP_FILL_OUTLIERS = 8
- WARP_INVERSE_MAP   = 16


### Gazebo log file

For checking the number of FPS of the window, open the `default.log` file in the server folder within `${HOME}/.gazebo` and search for `FPS` or `[GazeboCameraViewerPlugin]`.
