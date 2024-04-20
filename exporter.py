import laspy

def export_gpx_file(latitudes, longitudes, altitudes, filename):
    with open(filename + '.gpx', 'w') as file:
        file.write('<?xml version="1.0" encoding="UTF-8"?>\n')
        file.write('<gpx version="1.1" creator="PetitPoucet">\n')
        for i in range(len(latitudes)):
            if latitudes[i] is None or longitudes[i] is None or altitudes[i] is None:
                continue
            else:
                # convert to decimal degrees
                decimal_degrees_lat = latitudes[i] / 2**31 * 180
                decimal_degrees_long = longitudes[i] / 2**31 * 180
                file.write('  <wpt lat="' + str(decimal_degrees_lat) + '" lon="' + str(decimal_degrees_long) + '">\n')
                file.write('    <ele>' + str(altitudes[i]) + '</ele>\n')
                file.write('  </wpt>\n')
        file.write('</gpx>')

def export_las_file(latitudes, longitudes, altitudes, filename):
    header = laspy.header.Header()
    outfile = laspy.file.File(filename)
    outfile.header = header
    outfile.x = longitudes
    outfile.y = latitudes
    outfile.z = altitudes
    outfile.close()

def export_xyz_file(latitudes, longitudes, altitudes, filename):
    with open(filename + '.xyz', 'w') as file:
        for i in range(len(latitudes)):
            if latitudes[i] is None or longitudes[i] is None or altitudes[i] is None:
                continue
            else:
                file.write(str(longitudes[i]) + ' ' + str(latitudes[i]) + ' ' + str(altitudes[i]) + '\n')
