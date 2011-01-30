#include "Buffalo.h"
#include "Gamepad.h"
#include "Sprite.h"
#include "Terrain.h"
#include "Game.h"

Buffalo herd[kMaxBuffalo];

static int mFrame = 0;
static int mTime = 0;

void buffInit() {
}


static void spawn(vector2f pos) {
	int i;
	for (i=0; i<kMaxBuffalo; ++i) {
		if (!herd[i].bActive)
			break;
	}


	if (herd[i].bActive)
		return;

	herd[i].scale = 1;
	herd[i].bActive = true;
	herd[i].pos = pos;
	herd[i].rad = 12*herd[i].scale;
	herd[i].facing = (Facing)(Facing_South+rand()%4);
	herd[i].state = (Buffalo::State)( rand()%((int)Buffalo::State_NumStates) );

	herd[i].stage = 1;
	herd[i].ageMs = 0;
	herd[i].hp = 1;
}

void buffReset() {
	memset(herd, 0, sizeof(Buffalo)*kMaxBuffalo);

	for (int i=0; i<kInitialBuffalo; ++i) {
		vector2f pos = vector2f((rand()%kWorldWidth)*32,(rand()%kWorldHeight)*32);
		spawn(pos);
		herd[i].bearing = 0;
	}
}

void buffRender(Sprite2d* sprite) {
	for (int i=0; i<kMaxBuffalo; ++i) {
		Buffalo& buff = herd[i];

		glPushMatrix();
			glTranslatef(buff.pos.x-16*buff.scale, buff.pos.y-16*buff.scale, 0);
			glScalef(buff.scale, buff.scale, 0);

		if (!buff.bActive) {
			sprite->draw(kBuffaloFrame+6);
			glPopMatrix();
			continue;
		}


		if (buff.state == Buffalo::State_Angry) {
			if ((Game::getInstance()->getTime()/666)%2)
				glColor3f(1.0, 0, 0);
			else
				glColor3f(1, 1, 1);
		}

		const int elderly = buff.stage == 4 ? 8 : 0;

		switch (buff.facing) {
			case Facing_South:
				sprite->draw(kBuffaloFrame+mFrame+elderly);
				break;
			case Facing_West:
				sprite->draw(kBuffaloFrame+mFrame+2+elderly);
				break;
			case Facing_North:
				sprite->draw(kBuffaloFrame+mFrame+4+elderly);
				break;
			case Facing_East:
				glPushMatrix();
				glTranslatef(32, 0, 0);
				glScalef(-1, 1, 1);
				sprite->draw(kBuffaloFrame+mFrame+2+elderly);
				glPopMatrix();
				break;
		}

			glColor3f(1, 1, 1);


		glPopMatrix();
	}

}


void buffUpdate(uint32_t elapsedMs, Gamepad* gamepad) {
	mTime += elapsedMs;
	mFrame = (mTime/300)%2;

	vector2f ppos = Game::getInstance()->getPlayerPos();

	for (int i=0; i<kMaxBuffalo; ++i) {
		Buffalo& buff = herd[i];
		if (!buff.bActive)
			continue;

        //target vector(angry)
		buff.ageMs += elapsedMs;

		if (buff.ageMs > kBuffaloAgeMs) {
			buff.ageMs -= kBuffaloAgeMs;
			if (buff.stage < 4) {
				buff.hp = ++buff.stage;
				if (buff.stage == 4) {
					spawn(buff.pos);
				}
			}
		}

		// smooth animate the grow
		// set scale according to health, 1:1 for now
		if (buff.hp > buff.scale) {
			buff.scale = buff.scale*0.75+buff.hp*0.25;
			buff.rad = buff.scale * 12;
		}

		vector2f dp = ppos-buff.pos;
		float mag = dp.length();

        vector2f pos_a(0,0);
        vector2f head_a(0,0);


        for(int j = 0;j<kMaxBuffalo;j++)
        {
            herd[i].vel = vector2f(sin(herd[i].bearing),cos(herd[i].bearing));

            if(i==j)
            {
                continue;
            }
            //get the average
            pos_a += herd[j].pos;
            head_a += vectorFromHeading(herd[j].bearing);

            vector2f dist = herd[j].pos - herd[i].pos;

            if(dist.length() < NEIGHBOR_THRESHOLD )
            {
                float  sep_b;


                sep_b = normalize(atan2f(dist.y,dist.x) + vector2f::angle((vector2f(0,1)),dp));

                 herd[i].bearing = mix(herd[i].bearing,sep_b,SEPARATION_WEIGHT);
            }



        }
        pos_a /= kMaxBuffalo - 1;
        head_a /= kMaxBuffalo - 1;

        pos_a = pos_a - herd[i].pos;

        float r;
        r = (float)rand()/RAND_MAX;
        r *= 2*PI;

        //target
        herd[i].bearing = mix(herd[i].bearing, normalize(atan2f(dp.y,dp.x)),TARGET_WEIGHT);

        //random element
        herd[i].bearing = mix(herd[i].bearing, r,RANDOM_WEIGHT);


        //average direction of the group
        herd[i].bearing = mix(herd[i].bearing,normalize(atan2f(head_a.y,head_a.x)),ALIGNMENT_WEIGHT);

        //center of group
        herd[i].bearing = mix(herd[i].bearing,normalize(atan2f(pos_a.y,pos_a.x)),-COHESION_WEIGHT);



		if (mag > kBuffDistThreshold)
			continue;

		vector2f shift;

		switch (buff.state) {
			case Buffalo::State_Grazing: {

				const float heading = (rand()/(float)RAND_MAX)*M_PI*2;
				buff.pos += vectorFromHeading(herd[i].bearing, kBuffSpeed);

//				const float heading = (rand()/(float)RAND_MAX)*M_PI*2;
//				shift = vectorFromHeading(heading, kBuffSpeed);
//				buff.pos += shift;
				break;
			}
			case Buffalo::State_Afraid: {

				//buff.pos -= vectorFromHeading(herd[i].bearing, kBuffSpeed);;

				shift = dp.normalized()*kBuffSpeed;
				buff.pos -= shift;

				break;
			}
			case Buffalo::State_Angry: {

				//buff.pos += vectorFromHeading(herd[i].bearing, kBuffSpeed);;

				shift = dp.normalized()*kBuffSpeed;
				buff.pos += shift;

				break;
			}
		}


		static const float kThird = 1.0/3.0;
		if (shift.y < -kThird) {
			buff.facing = Facing_North;
		} else if (shift.y > kThird) {
			buff.facing = Facing_South;
		} else if (shift.x < 0) {
			buff.facing = Facing_West;
		} else {
			buff.facing = Facing_East;
		}


	}

}

void buffHit(int index) {
	if (!herd[index].bActive)
		return;

	--herd[index].hp;

	if (herd[index].hp <= 0) {
		herd[index].bActive = false;
	}
}

float normalize(float i)
{
   // i+= PI/2;
    while(i>2*PI)
    {
        i-=2*PI;
    }
    while(i<0)
    {
        i+=2*PI;
    }

    return i;
}

//uses bearings. Probably dumb
float mix(float base, float mix, int weight)
{
    float value;

    value = base * (weight - 1);
    value += mix;
    value /= weight;

    //calculate a vector length weight, sum them
    vector2f vbase;
    vector2f vmix;

    vbase.x = STATIC_WEIGHT*sin(base);
    vbase.y = STATIC_WEIGHT*cos(base);

    vmix.x = weight*sin(mix);
    vmix.y = weight*cos(mix);

    vector2f result = (vbase + vmix);


    return normalize(atan2f(result.y,result.x));
}
