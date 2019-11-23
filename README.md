# Cat-Maus-Simulation-GameVR

![VR game demo](https://user-images.githubusercontent.com/42900802/69480443-5f01da80-0dff-11ea-87cd-5db4a7996dd0.png)

Combined OpenGL and openCV. Here is a link for the demo: https://www.youtube.com/watch?v=sTga34dPvfI&t=2s.

There are cats and mice in this world in a 3D space. The mice are to be eaten by the cats. A "eat" happens when the 3D distance between a cat and a mouse is less than a given threshold (right now it is 25).

You have two modes: Simulation or a VR game. The software starts out with the simulation. If all of the mice are eaten in one setting, the modes automatically switches to the other one. The modes could be changed by pressing the key "n". The cats are run by 2 types of actions:

###### Greedy-Action
The cats find the nearest mouse in the 3D space and try to eat it.

###### Random Action
The cats move randomly, without respect to the position of the mice. 

The switch between a greedy and a random action can be done by pressing "m".

#### Simulation

This class deals with running the 3D world. The cats try to eat the mice. If they do, the mouse dies and the cat increases in size. If the cat eats more than a certain number of mice, then it dies and mice are added in its place. If all of the cats die, then 5 more cats are added to the system. Depending on what is the mice-eaten-cat-death-threshold, the game might never finish (i.e. if a cat dies after only eating 3 mice, then the simulation will never happen).

![VR simu](https://user-images.githubusercontent.com/42900802/69480563-61186900-0e00-11ea-8356-4b13bf901880.png)

#### VR game
It is a feature of the Simulation class. You will compete against an other computer controlled cat to see which one wins. The opponent cat could be operating on a greedy-action or a random-action depending on your settings (and you can always switch). You put a distinct colored object in the square below. Then press "Esc". 

![VR](https://user-images.githubusercontent.com/42900802/69480541-33332480-0e00-11ea-9115-cb86038b0e9a.png)

The webcam will trace that color in real-time and move your cat in the x-y frame with respect to the position of object in the web-cam frame. The depth of the cat could be controlled by using the the "r" and "f" keys to increase and decrease it respectively. 

        ![VR thumb](https://user-images.githubusercontent.com/42900802/69480648-53afae80-0e01-11ea-9f34-c621c9d1dcd1.png)

In the terminal it prints out the score for each cat once one of the mice is eaten.

Some other openGL features that have been implemented are:
-> Phong Illumination model

![VR phong](https://user-images.githubusercontent.com/42900802/69480591-a5a40480-0e00-11ea-9cd2-962db9f32fa6.png)

-> Hierarchical animated object

![VR h](https://user-images.githubusercontent.com/42900802/69480609-e13ece80-0e00-11ea-88b1-661b2f0b43a4.png)

###### Code Structure?
![VR Code](https://user-images.githubusercontent.com/42900802/69480625-09c6c880-0e01-11ea-8c01-70fc96260b2e.png)

###### How to run?
You will need a linux environment.
Run in terminal "make run".
