import csv
import os

dataFolder = "./res/" # local folder
fileName = "train.tsv" # resource
itemsLimit = 3000 # numbers of items sampled

items_fd = open(os.path.join(dataFolder, fileName))

for i, item in enumerate(itemReader):
  
	item = {featureName:featureValue.decode('utf-8') for featureName,featureValue in item.iteritems()} # one row
	print i, item
	
# example: item = {'price':u'0', 'number':u'12005'}
