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

* [Qt](http://qt-project.org/) 4.8
* [VTK](http://www.vtk.org/) 5.10.1 (with [Qt support](http://www.vtk.org/Wiki/VTK/Tutorials/QtSetup)!)
* [Boost](http://www.boost.org/) >=1.47
* [Git](http://git-scm.com/) optional, only required if the auto download feature is used (see below)

The following requirements, being part of the so-called 'ViennaStar' collection, 
can be automatically downloaded by the build system, see the building instructions at the end.

* [ViennaFVM](https://github.com/viennafvm/viennafvm-dev)
* [ViennaGrid](https://github.com/viennagrid/viennagrid-dev)
* [ViennaData](https://github.com/viennadata/viennadata-dev)
* [ViennaMath](https://github.com/viennamath/viennamath-dev)
* [ViennaCL](https://github.com/viennacl/viennacl-dev)
* [ViennaMaterials](https://github.com/viennamaterials/viennamaterials-dev)
* [ViennaMesh](https://github.com/viennamesh/viennamesh-dev)
* [ViennaMini](https://github.com/viennamini/viennamini-dev)


Building instructions 
--------------------------

To build the developer version of ViennaMOS, simply clone the repository and issue the following suggested commands (the following steps are for Unix-based systems):

<pre>
$> cd viennamos-dev  # the checked-out GIT folder 
$> mkdir build       # the build folder
</pre>

Configure the build, default build type is the 'optimized/release' mode:
<pre>
$> cmake ..  
</pre>

Note that this default configuration requires the paths to the above mentioned ViennaStar libraries 
to be available via specific environment variables, i.e., 
<pre>
  VIENNAFVMPATH, VIENNAGRIDPATH, .. etc.
</pre>

If you do not have these libraries available, you can tell the build system
to automatically download and build the tools from their respective sources:

<pre>
$> cmake -D DOWNLOAD=ON ..
</pre>

If you want to build in debug mode, use:
<pre>
$> cmake -DCMAKE_BUILD_TYPE=DEBUG ..  
</pre>

Now build and install the executable and libraries (install folder is a subfolder of the build folder):
<pre>
$> make -j4  # adjust to your CPU core count for efficient parallel building
$> make install
</pre>
