// MIT License
// 
// Copyright(c) 2020 Arthur Bacon and Kevin Dill
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Mob.h"

#include "Constants.h"
#include "Game.h"
#include "Player.h"

#include <algorithm>
#include <vector>


Mob::Mob(const iEntityStats& stats, const Vec2& pos, bool isNorth)
    : Entity(stats, pos, isNorth)
    , m_pWaypoint(NULL)
{
    assert(dynamic_cast<const iEntityStats_Mob*>(&stats) != NULL);
}

void Mob::tick(float deltaTSec)
{
    // Tick the entity first.  This will pick our target, and attack it if it's in range.
    Entity::tick(deltaTSec);

    // if our target isn't in range, move towards it.
    if (!targetInRange())
    {
        move(deltaTSec);
    }
}

void Mob::move(float deltaTSec)
{
    // If we have a target and it's on the same side of the river, we move towards it.
    //  Otherwise, we move toward the bridge.
    bool bMoveToTarget = false;
    if (!!m_pTarget)
    {    
        bool imTop = m_Pos.y < (GAME_GRID_HEIGHT / 2);
        bool otherTop = m_pTarget->getPosition().y < (GAME_GRID_HEIGHT / 2);

        if (imTop == otherTop)
        {
            bMoveToTarget = true;
        }
    }

    Vec2 destPos;
    if (bMoveToTarget)
    { 
        m_pWaypoint = NULL;
        destPos = m_pTarget->getPosition();
    }
    else
    {
        if (!m_pWaypoint)
        {
            m_pWaypoint = pickWaypoint();
        }
        destPos = m_pWaypoint ? *m_pWaypoint : m_Pos;
    }

    // Actually do the moving
    Vec2 moveVec = destPos - m_Pos;
    Vec2 movedVec(0, 0);
    float distRemaining = moveVec.normalize();
    float moveDist = m_Stats.getSpeed() * deltaTSec;

    // if we're moving to m_pTarget, don't move into it
    if (bMoveToTarget)
    {
        assert(m_pTarget);
        distRemaining -= (m_Stats.getSize() + m_pTarget->getStats().getSize()) / 2.f;
        distRemaining = std::max(0.f, distRemaining);
    }

    if (moveDist <= distRemaining)
    {
        movedVec += moveVec * moveDist;
    }
    else
    {
        movedVec += moveVec * distRemaining;

        // if the destination was a waypoint, find the next one and continue movement
        if (m_pWaypoint)
        {
            m_pWaypoint = pickWaypoint();
            destPos = m_pWaypoint ? *m_pWaypoint : m_Pos;
            moveVec = destPos - m_Pos;
            moveVec.normalize();
            movedVec += moveVec * distRemaining;
        }
    }

    // PROJECT 2: This is where your collision code will be called from
    //Mob* otherMob = checkCollision();

    Mob* mobs[20];
    int collisions = 0;

    checkCollision2(mobs, collisions);


    if (collisions) {
        Vec2 prop = processCollision2(mobs, collisions, deltaTSec);
        movedVec = prop;

        printf("Physics force %s X:%2.2f, Y:%2.2f\n", this->getStats().getName(), movedVec.x, movedVec.y);
    }

    m_Pos += movedVec;

    if (m_Pos.x > SCREEN_WIDTH_PIXELS)
    {
        m_Pos.x = SCREEN_WIDTH_PIXELS;
    }

    if (m_Pos.x < 0)
    {
        m_Pos.x = 0;
    }

    if (m_Pos.y > SCREEN_HEIGHT_PIXELS)
    {
        m_Pos.y = SCREEN_HEIGHT_PIXELS;
    }

    if (m_Pos.y < 0)
    {
        m_Pos.y = 0;
    }
}

const Vec2* Mob::pickWaypoint()
{
    float smallestDistSq = FLT_MAX;
    const Vec2* pClosest = NULL;

    for (const Vec2& pt : Game::get().getWaypoints())
    {
        // Filter out any waypoints that are behind (or barely in front of) us.
        // NOTE: (0, 0) is the top left corner of the screen
        float yOffset = pt.y - m_Pos.y;
        if ((m_bNorth && (yOffset < 1.f)) ||
            (!m_bNorth && (yOffset > -1.f)))
        {
            continue;
        }

        float distSq = m_Pos.distSqr(pt);
        if (distSq < smallestDistSq) {
            smallestDistSq = distSq;
            pClosest = &pt;
        }
    }

    return pClosest;
}

// PROJECT 2: 
//  1) return a vector of mobs that we're colliding with
//  2) handle collision with towers & river 
Mob* Mob::checkCollision() 
{
    const Player& northPlayer = Game::get().getPlayer(true);
    for (const Entity* pOtherMob : northPlayer.getMobs())
    {
        if (this == pOtherMob) 
        {
            continue;
        }

        // PROJECT 2: YOUR CODE CHECKING FOR A COLLISION GOES HERE
    }

    // Collision counts and forceX,Y per collsion
    int nCollisions = 0;
    int forceX[20] = { 0 }, forceY[20] = { 0 };
    const Entity* collisions[20] = { nullptr };

    const Player& southPlayer = Game::get().getPlayer(false);
    for (Entity* pOtherMob : southPlayer.getMobs())
    {
        if (this == pOtherMob)
        {
            continue;
        }

        // PROJECT 2: YOUR CODE CHECKING FOR A COLLISION GOES HERE

        // Get stats from this and the other mob object
        const iEntityStats& thisStats = this->getStats();
        const iEntityStats& otherStats = pOtherMob->getStats();

        float mass1 = thisStats.getMass();
        float mass2 = otherStats.getMass();

        if (mass1 >= mass2)
            continue; //ignores when mass1 >= mass2

        Vec2 pos1 = this->getPosition();
        Vec2 pos2 = pOtherMob->getPosition();

        int radius1 = (int)thisStats.getSize() / 2;
        int radius2 = (int)otherStats.getSize() / 2;

        int diffX = (int)abs(pos2.x - pos1.x);
        int diffY = (int)abs(pos2.y - pos1.y);


        bool bCollision = false;
        if (diffX < radius2)
        {
            forceX[nCollisions] = radius2 - diffX;
            bCollision = true;
        }

        if (diffY < radius2)
        {
            forceY[nCollisions] = radius2 - diffY;
            bCollision = true;
        }

        if (bCollision)
        {
            collisions[nCollisions] = pOtherMob;
            nCollisions++;
            return static_cast<Mob*>(pOtherMob);
        }
    }

    return NULL;
}

// Extended version to check multiple objects in collision
void Mob::checkCollision2(Mob** collisionObjs, int& collisionCount)
{
    // Collision counts and forceX,Y per collsion
    float forceX[20] = { 0 }, forceY[20] = { 0 };

    const Player& northPlayer = Game::get().getPlayer(true);
    for (Entity* pOtherMob : northPlayer.getMobs())
    {
        if (this == pOtherMob)
        {
            continue;
        }

        // PROJECT 2: YOUR CODE CHECKING FOR A COLLISION GOES HERE

        // Get stats from this and the other mob object
        const iEntityStats& thisStats = this->getStats();
        const iEntityStats& otherStats = pOtherMob->getStats();

        float mass1 = thisStats.getMass();
        float mass2 = otherStats.getMass();



        Vec2 pos1 = this->getPosition();
        Vec2 pos2 = pOtherMob->getPosition();

        float radius1 = thisStats.getSize() / 2.0f;
        float radius2 = otherStats.getSize() / 2.0f;

        float diffX = pos2.x - pos1.x;
        float diffY = pos2.y - pos1.y;

        bool bCollision = false;
        if (abs(diffX) < radius2 && abs(diffY) < radius2)
        {
            forceX[collisionCount] = radius2 - abs(diffX);
            forceY[collisionCount] = radius2 - abs(diffY);
            bCollision = true;
        }

        if (bCollision)
        {
            if (mass1 > mass2)
            {
                // When mass1 greater than mass 2, pushes the other mob and continue;
                if (diffX >= 0.0f)
                {
                    static_cast<Mob*>(pOtherMob)->m_Pos.x -= forceX[collisionCount];
                }
                else
                {
                    static_cast<Mob*>(pOtherMob)->m_Pos.x += forceX[collisionCount];
                }

                if (diffY >= 0.0f)
                {
                    static_cast<Mob*>(pOtherMob)->m_Pos.y -= forceY[collisionCount];
                }
                else
                {
                    static_cast<Mob*>(pOtherMob)->m_Pos.y += forceY[collisionCount];
                }
                
                continue; 
            }

            collisionObjs[collisionCount] = static_cast<Mob*>(pOtherMob);
            collisionCount++;
        }
    }



    const Player& southPlayer = Game::get().getPlayer(false);
    for (Entity* pOtherMob : southPlayer.getMobs())
    {
        if (this == pOtherMob)
        {
            continue;
        }

        // PROJECT 2: YOUR CODE CHECKING FOR A COLLISION GOES HERE

        // Get stats from this and the other mob object
        const iEntityStats& thisStats = this->getStats();
        const iEntityStats& otherStats = pOtherMob->getStats();

        // Get mass, position, radius(half of square size) and differenceX,Y
        float mass1 = thisStats.getMass();
        float mass2 = otherStats.getMass();

        Vec2 pos1 = this->getPosition();
        Vec2 pos2 = pOtherMob->getPosition();

        float radius1 = thisStats.getSize() / 2.0f;
        float radius2 = otherStats.getSize() / 2.0f;

        float diffX = pos2.x - pos1.x;
        float diffY = pos2.y - pos1.y;

        bool bCollision = false;
        if (abs(diffX) < radius2 && abs(diffY) < radius2)
        {
            forceX[collisionCount] = radius2 - abs(diffX);
            forceY[collisionCount] = radius2 - abs(diffY);
            bCollision = true;
        }

        if (bCollision)
        {
            if (mass1 > mass2)
            {
#if 0
                if (diffX >= 0.0f)
                {
                    static_cast<Mob*>(pOtherMob)->m_Pos.x -= forceX[collisionCount];
                }
                else
                {
                    static_cast<Mob*>(pOtherMob)->m_Pos.x += forceX[collisionCount];
                }
#endif

                if (diffY >= 0.0f)
                {
                    static_cast<Mob*>(pOtherMob)->m_Pos.y -= forceY[collisionCount];
                }
                else
                {
                    static_cast<Mob*>(pOtherMob)->m_Pos.y += forceY[collisionCount];
                }

                continue; //ignores when mass1 >= mass2
            }

            collisionObjs[collisionCount] = static_cast<Mob*>(pOtherMob);
            collisionCount++;
        }
    }
}

void Mob::processCollision(Mob* otherMob, float deltaTSec)
{
    // PROJECT 2: YOUR COLLISION HANDLING CODE GOES HERE
}

Vec2 Mob::processCollision2(Mob **mobs, int mobsCount, float deltaTSec)
{
    // PROJECT 2: YOUR COLLISION HANDLING CODE GOES HERE
    // Collision counts and forceX,Y per collsion
    int forceX[20] = { 0 }, forceY[20] = { 0 };
    int signX[20] = { 0 }, signY[20] = { 0 };
    for (int i = 0; i < mobsCount; i++)
    {
        Mob* pOtherMob = mobs[i];
        const iEntityStats& thisStats = this->getStats();
        const iEntityStats& otherStats = pOtherMob->getStats();

        Vec2 pos1 = this->getPosition();
        Vec2 pos2 = pOtherMob->getPosition();

        int radius1 = (int)thisStats.getSize() / 2;
        int radius2 = (int)otherStats.getSize() / 2;

        int diffX = (int)(pos2.x - pos1.x);

        if (diffX >= 0)
            signX[i] = -1;
        else
            signX[i] = 1;

        int diffY = (int)(pos2.y - pos1.y);
        if (diffY >= 0)
            signY[i] = -1;
        else
            signY[i] = 1;

        forceX[i] = radius2 - abs(diffX);
        forceY[i] = radius2 - abs(diffY);
    }

    int maxForceX = -1, maxForceY = -1;
    int index = 0;
    int masses1[20] = { 0 }, masses2[20] = { 0 };
    for (int i = 0; i < mobsCount; i++)
    {
        Mob* pOtherMob = mobs[i];
        const iEntityStats& thisStats = this->getStats();
        const iEntityStats& otherStats = pOtherMob->getStats();

        masses1[i] = (int)thisStats.getMass();
        masses2[i] = (int)otherStats.getMass();

        Vec2 pos1 = this->getPosition();
        Vec2 pos2 = pOtherMob->getPosition();

        int radius1 = (int)thisStats.getSize() / 2;
        int radius2 = (int)otherStats.getSize() / 2;

        int diffX = (int)(pos2.x - pos1.x);
        int diffY = (int)(pos2.y - pos1.y);

        int forceX = radius2 - abs(diffX);
        int forceY = radius2 - abs(diffY);
        if (maxForceX < forceX)
            maxForceX = forceX;

        if (maxForceY < forceY)
        {
            maxForceY = forceY;
            index = i;
        }
    }

    Vec2 collisionVec;
    collisionVec.x = (float)signX[index] * forceX[index];
    collisionVec.y = (float)signY[index] * forceY[index];

    int mass1 = masses1[index];
    int mass2 = masses2[index];

    // When same mass objects collide applies avoidance steering
    if (mass1 == mass2)
    {
        Mob* pOtherMob = mobs[index];
        m_Pos.x += (this->getStats().getSize());
        pOtherMob->m_Pos.x -= (this->getStats().getSize());

        collisionVec.x = 0.0f;
        collisionVec.y = 0.0f;
        return collisionVec;
    }

    // no feedback from building with infinite mass
    if (mass2 > 999999)
    {
        collisionVec.x = 0.0f;
        collisionVec.y = 0.0f;
    }

    return collisionVec;
}

