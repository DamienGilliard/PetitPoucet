import importer
import exporter
import open3d

filename = 'Van_Sansepolcro_naar_Urbino_course'
lat, long, alt = importer.extract_lat_long_alt(filename + '.fit')
# importer.print_fit_file(filename + '.fit')

pointcloud = open3d.geometry.PointCloud()
for i in range(len(alt)):
    if lat[i] is None or long[i] is None or alt[i] is None:
        continue
    else:
        pointcloud.points.append([lat[i], long[i], alt[i]])
open3d.visualization.draw_geometries([pointcloud])

exporter.export_xyz_file(lat, long, alt, filename)
exporter.export_gpx_file(lat, long, alt, filename)