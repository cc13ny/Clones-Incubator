'''
  About GAL EEG Detection: https://www.kaggle.com/c/grasp-and-lift-eeg-detection
  Description: pickle data
'''

import pandas as pd
from glob import glob
from pandas import DataFrame as df

#-------------------Train Pickle------------------------
fnames = glob('./train/subj[1-9]_series*.csv')
for f in fnames:
    data = pd.read_csv(f, index_col=0)
    data.to_pickle('./train_df' + f[7:-4])

#--------------------Val Pickle-------------------------
fnames = glob('./train/subj1[0-1]_series*.csv')
for f in fnames:
    data = pd.read_csv(f, index_col=0)
    data.to_pickle('./val_df' + f[7:-4])
