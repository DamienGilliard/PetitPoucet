import fitparse

def print_fit_file(file_path):
    fitfile = fitparse.FitFile(file_path)
    for record in fitfile.get_messages('record'):
        for record_data in record:
            print(record_data.name, record_data.value, record_data.units)

def extract_lat_long_alt(file_path):
    fitfile = fitparse.FitFile(file_path)
    latitudes = []
    longitudes = []
    altitudes = []
    for record in fitfile.get_messages('record'):
        lat = None
        long = None
        alt = None
        for record_data in record:
            if record_data.name == 'position_lat':
                lat = record_data.value
            elif record_data.name == 'position_long':
                long = record_data.value
            elif record_data.name == 'altitude':
                alt = record_data.value
            latitudes.append(lat)
            longitudes.append(long)
            altitudes.append(alt)
    return latitudes, longitudes, altitudes