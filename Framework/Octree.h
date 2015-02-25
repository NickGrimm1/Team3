#include "T3Vector3.h"
#include "GameEntity.h"
#include "GameClass.h"
#include <set>
const int MAX_OCTREE_DEPTH = 6;
const int MIN_BALLS_PER_OCTREE = 3;
const int MAX_BALLS_PER_OCTREE = 6;
enum Wall {WALL_LEFT, WALL_RIGHT, WALL_FAR, WALL_NEAR, WALL_TOP, WALL_BOTTOM};
struct BallPair {
    GameEntity* ball1;
    GameEntity* ball2;
};

//Stores a ball and a wall
//struct BallWallPair {
//    GameEntity* ball;
//    Wall wall;
//};
//Our data structure for making collision detection faster
class Octree {
	public:
        //Constructs a new Octree.  c1 is (minimumX, minimumY, minimumZ), c2 is (maximumX, maximumY,
        //maximumZ), and d is the depth, which starts at 1.
        Octree(T3Vector3 c1, T3Vector3 c2, int d) {
            corner1 = c1;
            corner2 = c2;
            center = (c1 + c2) / 2;
            depth = d;
            numBalls = 0;
            hasChildren = false;
        }
		 ~Octree() {
            if (hasChildren) {
                destroyChildren();
            }
        }
		   void add(GameEntity* ball);
		   void remove(GameEntity* ball);
		   void ballMoved(GameEntity* ball, T3Vector3 oldPos);
		    void potentialBallBallCollisions(vector<BallPair> &collisions);
    private:
        T3Vector3 corner1; //(minimumX, minimumY, minimumZ)
        T3Vector3 corner2; //(maximumX, maximumY, maximumZ)
        T3Vector3 center;//((minimumX + maximumX) / 2, (minimumY + maximumY) / 2, (minimumZ + maximumZ) / 2)
		/* The children of this, if this has any.  children[0][*][*] are the
         * children with x coordinates ranging from minimumX to centerX.
         * children[1][*][*] are the children with x coordinates ranging from
         * centerX to maximumX.  Similarly for the other two dimensions of the
         * children array.
         */
        Octree *children[2][2][2];
		//Whether this has children
        bool hasChildren;
        //The balls in this, if this doesn't have any children
        set<GameEntity*> balls;
        //The depth of this in the tree
        int depth;
        //The number of balls in this, including those stored in its children
        int numBalls;
		//Adds a ball to or removes a from one to the children of this
        void fileBall(GameEntity* ball, T3Vector3 pos, bool addBall);
		void haveChildren() ;
		void collectBalls(set<GameEntity*> &bs);
		void destroyChildren() ;
		void remove(GameEntity* ball, T3Vector3 pos);

};
		