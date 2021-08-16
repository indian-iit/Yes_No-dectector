# Yes_No-dectector
 Identifying whether the word present in given recorded file is "yes" or "no".
 Input:A Recorded file which is converted to ASCII text file is feeded as input to the programe. The Recorded file as a single word, the
      word may be "yes" or "no".
Output:Programe will predict the the word present in the text file and print it.

 Follow the below steps to run the project:
step 1:Download the zip file and extract it.
step 2:Open Microsoft visual studio.
step 3:Search for the folder which is been extracted from the downloaded zip.
step 4:Click on the source files and open assignmnet1_yes.cpp file
step 5:press function+F5 to run the programe.

Files Used:
#To calculate the dcshift i have reduced my microphone volume to 0 and recorded the audio,the file is named as dc.txt.
#Have recorded 5 sample audio files, named as Sample_1(No).txt,Sample_2(Yes).txt,Sample_3(No).txt,Sample_4(Yes).txt,Sample_5(No).txt which have words "No",'Yes","No","Yes","No" respectively.
#While running the programe user is given the choice of selecting the file he wishesfor among those 5 files.

Preprocessing:
1)Dcshift: We have calculated the dcshift by taking average all values present in c.txt.
2)Wave Normalisation: We have considered our maximum amplitude as 5000, which means if the max value present in the recording is more than 5000
                    then we will reduce amplitude of the wave using normalisation factor(Limit/max value).Similalary if the max value is less than 
                    5000 we will increase the amplitude of wave form.At the same time we also remove dcshift from the values in the wave.
3)Computing energy and zcr:We will consider short term energy, by considering 100 samples as a single frame. We compute average energy and zcr for
                           these frames.
4)Ambient Noise:We will consider initial 10 frames of the file as Ambient noise as no word will be spoken in those 10 frames most probably.
                We will store the energy of ambient noise which will be helpful in further compuation.


Marking Starting and Ending of word:
#We will mark  the current frame as begining of the word if the energy of next 3 consecutive frames exceeds the 7* energy of ambient noise.
#We will mark  the current frame as ending of the word if the energy of next 3 consecutive frames are less the 3* energy of ambient noise.

Final Decision:
After marking the start and end of the word, if more than  30% of the word contains high zcr then we say it is "YES" beacuse "Yes" has is ending with fricative sound.
Frecative sounds imply  high ZCR. If ZCR is low it is predicted as "No".

 
