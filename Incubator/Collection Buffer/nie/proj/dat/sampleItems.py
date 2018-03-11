import csv
import os
import random as rnd

dataFolder = "./res/" # local folder
fileName = "train.tsv" # resource
itemsLimit = 3000 # numbers of items sampled

with open(os.path.join(dataFolder, fileName)) as items_fd:
  logging.info("Sampling...")
  
  if itemsLimit:
    cntReader = csv.DictReader(items_fd, delimiter='\t', quotechar='"') # "tsv"files as an example
    numItems = 0
    
    for row in countReader:
      numItems += 1
      items_fd.seek(0)        
      rnd.seed(0)
      sampleIndexes = set(rnd.sample(range(numItems),itemsLimit))
  
  logging.info("Sampling done.")
      
# sampleIndexes: the randomized indexes of rows
