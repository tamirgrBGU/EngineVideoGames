# EngineVideoGames
Video Games programming reposetory

keybindings-
case GLFW_KEY_ESCAPE:       will close the game
case GLFW_KEY_RIGHT:        rotate snake right
case GLFW_KEY_LEFT:         rotate snake left
case GLFW_KEY_SPACE:        pause&unpause game
case GLFW_KEY_KP_SUBTRACT:  zoom out
case GLFW_KEY_KP_ADD:       zoom in
case GLFW_KEY_P:            will change the drawing mode
case GLFW_KEY_F1:           in case it is too hard for you to complete the level press this button to past to the next level
case GLFW_KEY_F2:           to enable mouse dragging of the map(default is disabled)
case GLFW_KEY_F3:           to enable reset the snake if it got terribly wrong
case GLFW_KEY_D:            will change the camera mode - can reset your camera in case you dragged it away

rules-
*The game will start paused, you can unpause with space, key D, arrow left and right.
*loading levels take at most 20 seconds, be patient.
*the menu buttons need a long click to get your input.
*the snake is hunting food for his lover snake. when the snake finds food, he becomes slightly faster.
*you should get all the available food in the map and touch the cave wall to proceed to the next level.
*if you touch a wall or an obstacle - you will start the level from the beginning.
*you must not fail.. or you will become the meal.
*In addition, you may not come home before you collected all the food, or your wife will be angry.

cool features-project overview
1. snake can crawl and interact with the map.
2. multiple levels - with a class that maintains them.
3. multiple themes - which include unique snakeskin, walls, food and obstacles for each level, with a class that maintains them.
4. shadow shads.
5. maps are saved as CSV, the ground can be multi-level with stairs for the snake to climb, special spawn zone, and special object zone.
6. food and obstacles are randomly generated and have animation.
7. skybox.
8. snake animations every couple of seconds- close eyes and tongue motion.
9. menu and in-game options.
10. The game has SMT(snake motion tracker) class that track after the snakehead rotation and pass it through his nodes which makes him looks like a snake, which supports 3d motion of the snake see 13 for more details.
11. Camera class that tracking the snake in the map and has mode upon his head, the camera can ignore small vibrations of the snake when it is rotating in location and view angle which improve the gaming experience.
12. the objects that lay on the map - walls, food, obstacles, cave and stairs not always check the intersection with the snake. we search for close objects to his head in his level (when there are stairs there are multiple levels) the location of his head, there is efficient DS that search for objects in fixed radius from his head and check intersection with them. if there is intersection specific function to the objects is enabled.
13. snake kind of can climb stairs in the current setup. there is level 5 that can be reached if you recompile the game and you can try it out.

known bugs-
1. sometimes when you come in 90 degrees to a surface the intersection detection is not working.
2. some random numbers can be too big to place in the map you may need to restart the whole game.