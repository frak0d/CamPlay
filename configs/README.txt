1. Create folder with same name as your Game's window title, ie. "configs/<GameTitle>"
2. Train a "image" model on https://teachablemachine.withgoogle.com/train/image with apropriate class names.
3. Download it as tensorflow hdf5 (.h5) model and convert to json using provided script. Then put the json in folder created in step 1.
4. Create keyboard actions corresponding to different pose classes in the text file actions.txt in same folder.
5. The format for keyboard actions is: [prediction_name] <xdotool args>