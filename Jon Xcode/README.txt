Term Project - Music Visualizer

Jonathan Hao  			UID # 203904896
Jonathan Nguy			UID # 603799761
Stephanie Sabatine  	UID # 803795328

For our term project, we implemented a music visualizer.  Sound files are read and played using the FMOD library.
Values denoting different decibels of the sound are extracted and used to adjust objects in time with the music.
Everything is done in real time, so the spheres on the screen will resize based on the sounds.

The user is able to use keyboard controls to adjust the camera to move around the perimeter of the world enclosing the visualizer.

All aspects of the course material up to and including texture mapping are included.

The following are the controls:
	'q', 'Q', or 'Esc' ........ : exit
	'w' ....................... : look downwards onto spheres
	's' ....................... : look upwards onto spheres
	'a' ....................... : changes horizontal angle
	'd' ....................... : changes horizontal angle
	'p'	....................... : pause or unpause music
	'i' ....................... : zoom in
	'o' ....................... : zoom out
	' ' (space) ............... : reset viewing angles

For the music library, we used FMOD Ex.
	This was used for all the things we had to do with the music.
	We also used it to help us read the song's spectrum.
    
For the advanced topic, we did collision detection on the spheres.
    Basically, whenever a sphere hits another one, it will turn red.
    We implemented this in lighting.h in the testCollisions function
    We basically checked each sphere with the others and checked to see if the overlapped
    
Future Improvements:
    Add more shapes that could better represent the sounds, rather than just spheres.
    Have a more accurate spectrum (rather than just a 256 sample size)
    Add mouse movements to navigate through the space
    Get more accurate dB values from the spectrum