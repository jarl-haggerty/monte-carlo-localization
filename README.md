This program is an example of a simulated robot in a 2D space using Monte Carlo Localization to determine it's position on a map.  It reads distances from a radial laser range finder and determines the odds of it being at a point given these readings.
Every point on the screen represents an estimate of the robots location and over time the estimates will converge on the actual position of the robot.

To compile it you need Boost, Bullet Physics Library, SDL, and OpenGL.
