# EngineVideoGames
Video Games programming reposetory

keybindings-
case GLFW_KEY_ESCAPE: 		will close the game
case GLFW_KEY_RIGHT: 		rotate snake right
case GLFW_KEY_LEFT: 		rotate snake left
case GLFW_KEY_SPACE:		pause&unpause game
case GLFW_KEY_KP_SUBTRACT: 	zoomout
case GLFW_KEY_KP_ADD:		zoomin
case GLFW_KEY_P: 	  		will change the drawing mode
case GLFW_KEY_F1:			in case it is too hard for you to complete the level press this button to past to the next level
case GLFW_KEY_D:			will change the camera mode

rules-
*loading levels take at most 20 seconds, be patient.
*the menu buttons need a long click to get your input.
*the snake is hunting food for his lover snake. when the snake find food, he become slightly faster.
*you should get all the available food in the map and touch the cave wall to proceed to next level.
*if you touch a wall or an obstecle - you will start the level from the begining.
*you must not fail.. or you will become the meal.
*In addition you may not come home before you collected all the food, or your wife will be angry.

coolFeatures-
1. snake can crawl and interact with the map
2. multiple levels
3. multiple themes - which include unique snake skin, walls, food and obstacles for each level
4. shadow shader
5. maps are saved as csv, the ground can be multi-level with stairs for the snake to climb, special spawn zone, and special object zone.
6. food and obstacles are random generated, and have animation.
7. skybox
8. snake animations every couple of sedonds- close eyes and toungh motion
9. menu and in game options

known bugs-
1. sometimes when you come in 90 degrees to a surface the intersection detection is not working
2. some random numbers can be too big to place in the map you may need to restart the whole game