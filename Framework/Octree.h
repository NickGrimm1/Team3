#include "Vector3.h"
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
        //Constructs a new Octree.  c1 is (minX, minY, minZ), c2 is (maxX, maxY,
        //maxZ), and d is the depth, which starts at 1.
        Octree(Vector3 c1, Vector3 c2, int d) {
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
		   void ballMoved(GameEntity* ball, Vector3 oldPos);
		    void potentialBallBallCollisions(vector<BallPair> &collisions);
    private:
        Vector3 corner1; //(minX, minY, minZ)
        Vector3 corner2; //(maxX, maxY, maxZ)
        Vector3 center;//((minX + maxX) / 2, (minY + maxY) / 2, (minZ + maxZ) / 2)
		/* The children of this, if this has any.  children[0][*][*] are the
         * children with x coordinates ranging from minX to centerX.
         * children[1][*][*] are the children with x coordinates ranging from
         * centerX to maxX.  Similarly for the other two dimensions of the
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
        void fileBall(GameEntity* ball, Vector3 pos, bool addBall);
		void haveChildren() ;
		void collectBalls(set<GameEntity*> &bs);
		void destroyChildren() ;
		void remove(GameEntity* ball, Vector3 pos);

};
		