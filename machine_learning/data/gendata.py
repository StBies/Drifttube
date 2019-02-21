import numpy as np
import matplotlib.pyplot as plt
import sys

if len(sys.argv) < 2:
    print("File path argument must be provided. Exiting.")
    exit(-1)
fname = sys.argv[1]

class Data:
    """ The Data class is basically a container for an array that contains the raw voltage
        This class offers a method to plot the event using matplotlib

        Author: Stefan Bieschke
        Date: 02/12/2018
    """
    def __init__(self,data_array,event_number):
        """ Constructor
            Initializes a new Data object with the raw data measured by a FADC given as parameter. Also sets an event number.

            Author: Stefan Bieschke
            Date: 02/12/2018

            Parameters
            ----------
            dataArray : array
                Array containing the raw voltage readings from the FADC
            eventNumber : int
                Number of the event
        """
        self._raw_data = data_array
        self._event_number = event_number
        
        
    def get_drift_time(self,threshold,calibrated):
        """ Returns the drift time of this event's Data in nanoseconds
            The DataSet must be calibrated prior to calculating the drift time.
            
            Author: Stefan Bieschke
            Date: 02/12/2018

            Parameters
            ----------
            threshold : float
                threshold in volts that must be undershot in order to find drift time

            calibrated : bool
                specifying if DataSet is calibrated

            Returns
            -------
            int
                Drift time in nanoseconds
        """
        if calibrated:
            for i in range(len(self._raw_data)):
                if self._raw_data[i] < threshold:
                    if i < 170:
                        return -100
                    else:
                        return 4 * (i - 170)
            return -100 # -680 ns for trigger time correction
            
    def get_array(self):
        """ Getter method for the raw array

            Author: Stefan Bieschke
            Date: 02/12/2018

            Returns
            -------
            array
                Array containing the raw voltage
        """
        return self._raw_data

    def find_min(self,beginning=-1, end=-1):
        """ Find the minimum of a data object in a given range specified via the
            parameters beginning and end. If no parameters are given, the global
            minimum and its position will be returned.

            Author: Stefan Bieschke
            Date: 02/21/2019

            Parameters
            ----------
            beginning: int
                first bin (NOT time in ns), where search for a minimum begins.
            end: int
                last bin (NOT time in ns), to search for a minimum

            Returns
            -------
            list
                [min_pos, min] position (bin) of minimum location and minimum voltage
        """
        #catch wrong input parameters
        if beginning == -1:
            beginning = 0
        elif beginning > len(self._raw_data):
            return [-1,-1] #error

        if end == -1:
            end = len(self._raw_data)
        elif end > len(self._raw_data):
            return [-1,-1] #error

        min_pos = beginning
        min_val = 0xFFFF
        for i in range(beginning, end):
            if self._raw_data[i] < min_val:
                min_val = self._raw_data[i]
                min_pos = i
                
        return [min_pos, min_val]


    def find_edges(self,threshold):
        """ Detects the positions of falling and rising edges at a given threshold voltage.
            The list is as long as pulses were found to exceed the threshold voltage.
            Each entry in the list is a 2-element list again, containing the position
            of the falling edge and the rising edge.
            Example:
                if only one (negative voltage) pulse is present and first exceeds the
                threshold in bin 13 and last exceeds it in bin 42, the result will be
                [[13,42]]

            Author: Stefan Bieschke
            Date: 02/21/2019

            Parameters
            ----------
            threshold: float
                threshold voltage (negative) that must be exceeded by a pulse

            Returns
            -------
            list
                list of 2-element lists containing positions of falling and rising edges for each identified pulse                

        """
        #Note: This is largely copied from my C++: https://github.com/StBies/Drifttube/blob/v2testing/src/DataProcessor.cpp (Function pulses_over_threshold(const Event& data, unsigned short threshold, size_t from, size_t to)
        #Only adapted to python
        edges = []
        if self._raw_data[0] <= threshold:
            first_is_rising = True
        else:
            first_is_rising = False
            
        pulse_ended = not first_is_rising
	#comment this if block when we don't want to count cases where the first edge is rising
        if first_is_rising:
            edges.append([0xFFFF,0]) #error for first is rising - results in negative time over threshold

	#from maximum drift time on: loop over the event to even higher drift times
        for i in range(len(self._raw_data)):
            #if-else switches a variable in order not to count a single pulse bin per bin
            if self._raw_data[i] <= threshold and pulse_ended:
                edges.append([4 * i,0])
                pulse_ended = False;
		#in case we don't want to count cases where the first edge is rising
		#elif self._raw_data[i] > threshold and not pulse_ended and len(edges) == 0:
	            #pulse_ended = true;
            elif self._raw_data[i] > threshold and not pulse_ended:
                edges[len(edges)-1][1] = 4 * i #ns
                pulse_ended = True
        
        if not pulse_ended:
            edges[len(edges)-1][1] = 0xFFFF #error for last rising edge missing
            
        return edges
    

    def plot_data(self):
        """ Plot the Data
            Plots the event's voltage pulse form using matplotlib with time [ns] on the x-axis and voltage [V] on the y-axis.
            
            Author: Stefan Bieschke
            Date: 02/12/2018
        """
        n_bins = len(self._raw_data)
        time_per_bin = 4 #ns
        time_bins = [time_per_bin * i for i in range(n_bins)]
        
        plt.title("Event #{} voltage".format(self._event_number))
        plt.xlabel("time [ns]")
        plt.ylabel("voltage [V]")
        plt.plot(time_bins,self._raw_data)
        plt.show()

    def plot_with_threshold(self,threshold):
        n_bins = len(self._raw_data)
        time_per_bin = 4 #ns
        time_bins = [time_per_bin * i for i in range(n_bins)]
        thr = [threshold] * n_bins
        
        plt.title("Event #{} voltage".format(self._event_number))
        plt.xlabel("time [ns]")
        plt.ylabel("voltage [V]")
        plt.plot(time_bins,self._raw_data)
        plt.plot(time_bins,thr)
        plt.show()

class DataSet:
    """ A collection of Data objects. This class offers some methods to pull out
    events, to ask for the size and, most important, to perform a ground calibration
    """
    def __init__(self,events):
        """ Constructor with an event list as parameter.
            Initializes a new DataSet object and sets the passed event list as its events. After construction, the DataSet size will be the same as the passed list's size.

            Author: Stefan Bieschke
            Date: 02/12/2018

            Parameters
            ----------
            events : list
                list of Data objects
        """
        self._n_events = len(events)
        self._is_calibrated = False
        self._events = events

    def get_event(self,event_no):
        """ Returns the event specified by the parameter eventNo as Data class object if at least eventNo events exist in the DataSet.

            Author: Stefan Bieschke
            Date: 02/12/2018

            Parameters
            ----------
            eventNo : int
                number of the requested Data object. Must be smaller than self.nEvents.

            Returns
            -------
            Data
                Data object containing the event
        """
        if event_no < self._n_events:
            return self._events[event_no]

    def get_size(self):
        """ Getter method for the size of the DataSet.
            Returns the size of the DataSet, thus the maximum number of an event that can be requested using the getEvent(...) method.
            
            Author: Stefan Bieschke
            Date: 02/12/2018

            Returns
            -------
            int
                Number of events stored in this DataSet object.
        """
        return self._n_events

    def is_calibrated(self):
        """ Returns the status of the calibration
            True if calibration was already performed, False else

            Returns
            -------
                bool
                    True if calibrated, False else

        """
        return self._is_calibrated

    def perform_ground_calibration(self):
        """ Perform a ground calibration on all data in this DataSet.
            After the ground calibration, ground potential should read around 0.0 Volts.

            Author: Stefan Bieschke
            Date: 02/12/2018
        """ 
        first_bin_contents = [0] * self.get_size()
        for i in range(self.get_size()):
            evt = self.get_event(i)
            first_bin_contents[i] = evt.get_array()[0]

        mean = np.mean(first_bin_contents)
        stddev = np.std(first_bin_contents)

        #apply calibration
        for evt in self._events:
            arr = evt.get_array()
            for bin in range(len(arr)):
                arr[bin] -= mean

        self._is_calibrated = True

        return mean, stddev

    def calculateEfficiency(self,threshold,list_of_drifttimes):
        """ Calculate the detection efficiency for a given threshold voltage.

            Author: Stefan Bieschke
            Date: 02/19/2018

            Parameters
            ----------
            threshold : float
                Threshold in volts for that the efficiency is calculated.
                Must be smaller than zero.

            Returns
            -------
                float, float
                    Efficiency [0,1], error [0,1]
                    
        """
        efficiency, error = 0, 0
        
        return efficiency, error

    def calculate_drift_time_spectrum(self,threshold,t_min=-1,t_max=-1):
        if t_min == -1:
            t_min = 0
        if t_max == -1:
            t_max = len(self._events[0].get_array()) * 4
        drifttimes = []
        for event in self._events:
            drifttimes.append(event.get_drift_time(threshold,dataset.is_calibrated()))
            
        n_bins = (t_max - t_min) / 4
        dt_spect = plt.hist(drifttimes, bins=int(n_bins), range = (t_min,t_max))
        
        return dt_spect


    def calculate_rt_relation(self,dt_spect):
        #count valid drift times
        n_valid = 0
        for element in dt_spect:
            n_valid += element
            
        rt = [0] * len(dt_spect)
        normalization = 18.15 / n_valid
        for i in range(1,len(dt_spect)):
            rt[i] = rt[i-1] + dt_spect[i] * normalization

        return rt


#----------------------------------------------------------------------
#                           Begin program execution
#----------------------------------------------------------------------
file = open(fname,'rb') #read binary mode
events = []

#Read first 8 Bytes
n_events = np.fromfile(file,np.int64,1)[0]
n_bins = np.fromfile(file,np.int32,1)[0]

#Read events from binary file
for i in range(n_events):
    data = np.fromfile(file,np.float64,n_bins)
    events.append(Data(data,i))

#Build DataSet object from events read above
dataset = DataSet(events)


mean, dev = dataset.perform_ground_calibration()
print("Noise sigma found to be: Sigma = {} mV".format(dev))

#TODO: Create drift time spectrum
dt_spect = dataset.calculate_drift_time_spectrum(-5 * dev,0,1600)         
plt.title("Drift time spectrum")
plt.xlabel("drift time [ns]")
plt.ylabel("#")
plt.show()

rt = dataset.calculate_rt_relation(dt_spect[0])
print("Plotting rt-relation")
plt.plot(dt_spect[1][:-1],rt)
plt.title("rt-relation")
plt.xlabel("drift time [ns]")
plt.ylabel("radius [mm]")
plt.show()
