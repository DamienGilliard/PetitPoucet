# PetitPoucet
PetitPoucet is a small executable creating a ply point cloud in world coordinate references and a .gpx file from a .fit file, from example from a bike gps or smartwatch.

The initial usecase is to create maps of unreferenced paths in forests.

The gpx file can be uploaded to openstreetmaps via the [openstreetmap website](https://www.openstreetmap.org/traces)
## Dependencies
we use :
* [fitparse](https://pythonhosted.org/fitparse/) 
* [laspy](https://laspy.readthedocs.io/en/latest/index.html)

## Note: 
consumer devices record gpx data at a low resolution (every 10-15s on a bicycle), and might not be fit for all applications.