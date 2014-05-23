import numpy as np
import scipy.io as sio
import pylab as pl
import itertools as it
import io
def ret_tru(inval):
    return true
def get_next_ind(in_file, sep_ln="&", predicate = ret_tru):
    lvals = []
    for line in in_file:
        line = line.strip()
        if line == sep_ln:
            break
        if predicate(line):
            lvals.append(line)
    return lvals



def split_list(inl, predicate):
    curl = []
    tl = []
    for v in inl:
        if(predicate(v)):
            if len(tl) > 0:
                curl.append(tl)
            tl = []
        else:
            tl.append(v)
    return curl



dat_file = open("data_out.out")
#do processing for each value in the output
ltest = get_next_ind(dat_file, "&", lambda x:(x.startswith("Func") or x.startswith("Ablation") or x.startswith("Time") or x.startswith("&&")))
while(len(ltest) != 0):
    ltest = split_list(ltest, lambda x:x == "&&")
    if(len(ltest) == 0):
            break
    fncstack = []
    timelist = ""
    abl_val=[]
#create numpy arrays
    for simval in ltest:
        if len(simval)==1:
            abl_val = np.fromstring(''.join(it.dropwhile(lambda x : not x.isdigit(), simval[0])), sep=' ')
            continue

        timestr = simval[0]
        fncstr = simval[1]
        timestr = it.dropwhile(lambda x: not x.isdigit(), timestr)
        timestr = ''.join(timestr)
        timelist = timelist + " " + timestr
        fncstr = it.dropwhile(lambda x: not x.isdigit(), fncstr)
        fncstr = ''.join(fncstr)
        fncstack.append(np.fromstring(fncstr, sep=' '))


    neur_mat = np.vstack(fncstack).transpose();
    time_vec = np.fromstring(timelist, sep=' ')
#write to .mat file

#create file name
    fbase = "abl_study_"
    for abl in abl_val:
        fbase = fbase + str(int(abl)) + "_"
    fbase = fbase[:-1]
    print fbase
    sio.savemat(fbase, {'abl_vals':np.array(abl_val), 'neur_series':neur_mat, 'time_vec':time_vec})
    ltest = get_next_ind(dat_file, "&", lambda x:(x.startswith("Ablation") or x.startswith("Func") or x.startswith("Time") or x.startswith("&&")))

