# SpectrumAnalyser
Example of the short-time Fourier transform of audio signals.
Program plots a real-time oscillogram and spectrogram of audio signal capturing it from an audio input device.

This program designed by using the Qt software framework version 5.4.

### Build for Windows.
  1. Download Qt Framework for Windows from http://www.qt.io/download/ (recommended the community version with  major version 5)
  2. Install Qt Framework on your system.
  3. Open .pro file in Qt-Creator IDE and build the program.
  4. Depending on the version of Qt License may need to copy binary files of the Qt library in the program directory for run it.
  
### Build for Linux.
  1. If Qt is not installed on your system, install Qt. See the documentation on http://www.qt.io/.
  2. Install Qt Creator (from software centre for Ubuntu)
  3. Build program using Qt Creator.
  
  NOTE: 
   + Install Qt Multimedia module if will appear the error " Unknown module(s) in QT: multimedia".

	 <code>$ apt-get install libqt5multimedia5</code>   
	
   + If program can't to detect any audio-devices then try to install Qt5 multimedia plugins
     
	 <code>$ apt-get install libqt5multimedia5-plugins</code>
	 
### Build for Android.
  1. Download and install Qt for Android from http://www.qt.io/download/
  2. Download and install JDK from http://www.oracle.com/technetwork/java/javase/downloads/index.html
  3. Download and install Android SDK form http://developer.android.com/sdk/index.html
  4. Install API version and USB driver for your Android device using android SDK manager
  5. Download and install NDK from http://developer.android.com/tools/sdk/ndk/index.html
  6. Download and install Apache ANT from http://ant.apache.org/bindownload.cgi
  7. Configure Qt Creator (Tools -> Options -> Android)
  8. Open project in Qt Creator and build a packet (apk) in according with your version of ABI.
  
NOTE: you can to download binary files of the program from distr directory.
