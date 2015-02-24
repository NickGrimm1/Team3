#include "Octree.h"


void Octree::add(GameEntity* ball) {
            numBalls++;
            if (!hasChildren && depth < MAX_OCTREE_DEPTH &&
                numBalls > MAX_BALLS_PER_OCTREE) {
                haveChildren();
            }
            
            if (hasChildren) {
                fileBall(ball, ball->GetPhysicsNode().GetPosition(), true);
            }
            else {
                balls.insert(ball);
            }
        }

 void Octree::remove(GameEntity* ball) {
            remove(ball, ball->GetPhysicsNode().GetPosition());
        }
 //Changes the position of a ball in this from oldPos to ball->pos
        void Octree::ballMoved(GameEntity* ball, Vector3 oldPos) {
            remove(ball, oldPos);
            add(ball);
        }

void Octree::potentialBallBallCollisions(vector<BallPair> &collisions) {
            if (hasChildren) {
                for(int x = 0; x < 2; x++) {
                    for(int y = 0; y < 2; y++) {
                        for(int z = 0; z < 2; z++) {
                            children[x][y][z]->
                                potentialBallBallCollisions(collisions);
                        }
                    }
                }
            }
            else {
                //Add all pairs (ball1, ball2) from balls
                for(set<GameEntity*>::iterator it = balls.begin(); it != balls.end();
                        it++) {
                    GameEntity *ball1 = *it;
                    for(set<GameEntity*>::iterator it2 = balls.begin();
                            it2 != balls.end(); it2++) {
                        GameEntity *ball2 = *it2;
                        //This test makes sure that we only add each pair once
                        if (ball1 < ball2) {
                            BallPair bp;
                            bp.ball1 = ball1;
                            bp.ball2 = ball2;
                            collisions.push_back(bp);
                        }
                    }
                }
            }
        }



void Octree::fileBall(GameEntity* ball, Vector3 pos, bool addBall) {
            //Figure out in which child(ren) the ball belongs

            for(int x = 0; x < 2; x++) {
                if (x == 0) {
					if (pos.x - ball->GetRenderNode().GetBoundingRadius()  < center.x) {
                        continue;
                    }
                }
                else if (pos.x + ball->GetRenderNode().GetBoundingRadius() < center.x) {
                    continue;
                }
                
                for(int y = 0; y < 2; y++) {
                    if (y == 0) {
                        if (pos.y - ball->GetRenderNode().GetBoundingRadius()  > center.y) {
                            continue;
                        }
                    }
                    else if (pos.y + ball->GetRenderNode().GetBoundingRadius()  < center.y) {
                        continue;
                    }
                    
                    for(int z = 0; z < 2; z++) {
                        if (z == 0) {
							if (pos.z - ball->GetRenderNode().GetBoundingRadius() > center.z) {
                                continue;
                            }
                        }
						else if (pos.z + ball->GetRenderNode().GetBoundingRadius() < center.z) {
                            continue;
                        }
                        
                        //Add or remove the ball
                        if (addBall) {
                            children[x][y][z]->add(ball);
                        }
                        else {
                            children[x][y][z]->remove(ball, pos);
                        }
                    }
                }
            }
        }
void Octree::haveChildren() {
            for(int x = 0; x < 2; x++) {
                float minX;
                float maxX;
                if (x == 0) {
                    minX = corner1.x;
                    maxX = center.x;
                }
                else {
                    minX = center.x;
                    maxX = corner2.x;
                }
                
                for(int y = 0; y < 2; y++) {
                    float minY;
                    float maxY;
                    if (y == 0) {
                        minY = corner1.y;
                        maxY = center.y;
                    }
                    else {
                        minY = center.y;
                        maxY = corner2.y;
                    }
                    
                    for(int z = 0; z < 2; z++) {
                        float minZ;
                        float maxZ;
                        if (z == 0) {
                            minZ = corner1.z;
                            maxZ = center.z;
                        }
                        else {
                            minZ = center.z;
                            maxZ = corner2.z;
                        }
                        
                        children[x][y][z] = new Octree(Vector3(minX, minY, minZ),
                                                       Vector3(maxX, maxY, maxZ),
                                                       depth + 1);
                    }
                }
            }
            
            //Remove all balls from "balls" and add them to the new children
            for(set<GameEntity*>::iterator it = balls.begin(); it != balls.end();
                    it++) {
                GameEntity* ball = *it;
				fileBall(ball, ball->GetPhysicsNode().GetPosition(), true);
            }
            balls.clear();
            
            hasChildren = true;
        }
void Octree::collectBalls(set<GameEntity*> &bs) {
			if (hasChildren) {
				for(int x = 0; x < 2; x++) {
					for(int y = 0; y < 2; y++) {
						for(int z = 0; z < 2; z++) {
							children[x][y][z]->collectBalls(bs);
						}
					}
				}
			}
			else {
				for(set<GameEntity*>::iterator it = balls.begin(); it != balls.end();
						it++) {
					GameEntity* ball = *it;
					bs.insert(ball);
				}
			}
		}
void Octree::destroyChildren() {
            //Move all balls in descendants of this to the "balls" set
            collectBalls(balls);
            
            for(int x = 0; x < 2; x++) {
                for(int y = 0; y < 2; y++) {
                    for(int z = 0; z < 2; z++) {
                        delete children[x][y][z];
                    }
                }
            }
            
            hasChildren = false;
        }
void Octree::remove(GameEntity* ball, Vector3 pos) {
            numBalls--;
            
            if (hasChildren && numBalls < MIN_BALLS_PER_OCTREE) {
                destroyChildren();
            }
            
            if (hasChildren) {
                fileBall(ball, pos, false);
            }
            else {
                balls.erase(ball);
            }
        }

