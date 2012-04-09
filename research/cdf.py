#!/usr/bin/env python

__all__ = ['extract_cdf', 'get_value_at', 'process_cdf_file',
           'remove_last_extension', 'write_cdf', 'write_cdf_info']

__author__ = 'Adelbert Chang (adelbertc@gmail.com)'

def extract_cdf(dataList):
    """
     Much thanks to this guy: http://www.srcco.de/v/cdf-plots-perf-eval
    """ 
    prob = 0
    amountOfData = len(dataList)
    cdf = [] # List of tuples (prob, data)

    for data in dataList:
        prob += 1.0 / amountOfData
        ret.append((prob, data))
    return cdf

def get_value_at(percentage, cdf):
    if (percentage > 1):
        percentage /= 100.0

    prev = 0
    for prob, data in cdf:
        # Only reliable if there is LOTS of data
        if prob > percentage:
            return prev
        prev = data
        
def process_cdf_file(filename):
    infile = open(filename)
    dataList = []
    for line in infile:
        # Assume relevant data is the last element of the line
        dataList.append(float((line.split())[-1]))
    infile.close()
    return sorted(dataList)
    
def remove_last_extension(filename):
    """
    Given a file such as foo.bar.ext, returns foo.bar
    Given a file without an extension (indicated by a dot somewhere),
        returns the filename back.
    """
    match = re.search(r'(\w+)\.\w+', filename[::-1])
    if match:
        extension = match.group(1)
        extensionLen = len(extension) + 1 # add 1 for the dot
        return filename[:-extensionLen]
    return filename

def write_cdf(cdf, outputFilename):
    outfile = open(outputFilename, 'w')
    for prob, data in cdf:
        outfile.write(str(prob) + ' ' + str(data) + '\n')
    outfile.close()

def write_cdf_info(cdf, outputFilename):
    outfile = open(outputFilename, 'w')
    i = 10
    while (i <= 100):
        outfile.write(str(i / 100.0) + ' ' + str(get_value_at(i, cdf)) + '\n')
        i += 10
    outfile.close()

if __name__ == '__main__':
    import sys
    if len(sys.argv) != 2:
        sys.exit('Usage: python %s <data file>' % sys.argv[0])
    
    print 'Processing command line args...'
    filename = sys.argv[1]
    fileprefix = remove_last_extension(filename)
    
    print 'Processing input file...'
    dataList = process_cdf_file(filename)
    
    print 'Extracting cdf...'
    cdf = extract_cdf(dataList)
    
    print 'Writing cdf...'
    write_cdf(cdf, fileprefix + '.cdf')
    write_cdf_info(cdf, fileprefix + '.cdfInfo')
    
    print 'Done!'