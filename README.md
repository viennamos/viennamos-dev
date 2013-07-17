ViennaMOS 
--------------------------

This is the developer repository of [ViennaMOS](http://viennamos.sourceforge.net/). 
Git snapshot packages and releases are available at [sourceforge](https://sourceforge.net/projects/viennamos/).


System requirements
--------------------------

* [Qt](http://qt-project.org/) 4.8
* [VTK](http://www.vtk.org/) 5.10.1 (with [Qt support](http://www.vtk.org/Wiki/VTK/Tutorials/QtSetup)!)
* [Boost](http://www.boost.org/) >=1.47



Building instructions 
--------------------------

To build the developer version of ViennaMOS, simply clone the repository and issue the following commands (the following steps are for Unix-based systems):

<pre>
$> cd viennamos-dev
$> mkdir build
$> cmake ..
$> make -j4
$> make install
</pre>



