ViennaMOS 
--------------------------

This is the developer repository of [ViennaMOS](http://viennamos.sourceforge.net/). 
ViennaMOS is currently in a prototype state. If you are interested in contributing
to this project, please leave comments to share your views on the most urgent tasks to
advance ViennaMOS in a productive state.
The source code is available under the GNU GPL.
Git snapshot packages and releases are available at [sourceforge](https://sourceforge.net/projects/viennamos/).


System requirements
--------------------------

* Ubuntu 14.04 (newer might give problems)
* 2-4 GB RAM (add swap space!)
* 20-30 GB Disk-Space
* [Qt](http://qt-project.org/) 4.8
* [VTK](http://www.vtk.org/) 5.8 - 5.10.1 (with [Qt support](http://www.vtk.org/Wiki/VTK/Tutorials/QtSetup)!)
* [Boost](http://www.boost.org/) >=1.47

<pre>
$> sudo apt-get install git wget g++ cmake libqt4-dev libboost1.55-all-dev
</pre>

Building instructions 
--------------------------

To build the developer version of ViennaMOS, simply clone the repository and issue the following suggested commands (the following steps are for Unix-based systems):

<pre>
$> git clone https://github.com/thesourcerer8/viennamos-dev/
$> cd viennamos-dev  # the checked-out GIT folder 
$> mkdir build       # the build folder
</pre>

Configure the build, default build type is the 'optimized/release' mode:
<pre>
$> cd build
$> cmake ..  
</pre>

or for configuring a build with debugging symbols use:
<pre>
$> cmake -DCMAKE_BUILD_TYPE=DEBUG ..  
</pre>

Now build and install the executable and libraries (install folder is a subfolder of the build folder):
<pre>
$> make -j4  # adjust to your CPU core count for efficient parallel building
$> make install
$> framework/viennamos
</pre>
