/************************************************************************/
/*                                                                      */
/* This file is part of VDrift.                                         */
/*                                                                      */
/* VDrift is free software: you can redistribute it and/or modify       */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.                                  */
/*                                                                      */
/* VDrift is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.                         */
/*                                                                      */
/* You should have received a copy of the GNU General Public License    */
/* along with VDrift.  If not, see <http://www.gnu.org/licenses/>.      */
/*                                                                      */
/************************************************************************/

#ifndef _AI_CAR_STANDARD_H
#define _AI_CAR_STANDARD_H

#include "ai_car.h"
#include "ai_factory.h"
#include "physics/carinput.h"
#include "graphics/scenenode.h"
#include "bezier.h"

#include <vector>
#include <list>
#include <map>

class CarDynamics;

class AiCarStandardFactory : public AiFactory
{
	AiCar * Create(const CarDynamics * car, float difficulty);
};

class AiCarStandard : public AiCar
{
public:
	AiCarStandard(const CarDynamics * new_car, float new_difficulty);

	~AiCarStandard();

	void Update(float dt, const CarDynamics cars[], const int cars_num);

#ifdef VISUALIZE_AI_DEBUG
	void Visualize();
#endif

private:
	float longitude_mu;			///< friction coefficient of the tire - longitude direction
	float lateral_mu;			///< friction coefficient of the tire - lateral direction
	const Bezier * last_patch;	///< last patch the car was on, used in case car is off track
	bool use_racingline;		///< true allows the AI to take a proper racing line

	struct OtherCarInfo
	{
		OtherCarInfo() : active(false) {}

		float horizontal_distance;
		float fore_distance;
		float eta;
		bool active;
	};
	std::map <const CarDynamics *, OtherCarInfo> othercars;

	void UpdateGasBrake();

	void CalcMu();

	float CalcSpeedLimit(const Bezier * patch, const Bezier * nextpatch, float friction, float extraradius);

	float CalcBrakeDist(float current_speed, float allowed_speed, float friction);

	void UpdateSteer();

	void AnalyzeOthers(float dt, const CarDynamics cars[], const int cars_num);

	///< returns a float that should be added into the steering wheel command
	float SteerAwayFromOthers();

	///< returns a float that should be added into the brake command. speed_diff is the difference between the desired speed and speed limit of this area of the track
	float BrakeFromOthers(float speed_diff);

	Bezier RevisePatch(const Bezier * origpatch, bool use_racingline);

	static float RateLimit(float old_value, float new_value, float rate_limit_pos, float rate_limit_neg);

	static const Bezier * GetCurrentPatch(const CarDynamics * c);

	static Vec3 GetPatchFrontCenter(const Bezier & patch);

	static Vec3 GetPatchBackCenter(const Bezier & patch);

	static Vec3 GetPatchDirection(const Bezier & patch);

	static Vec3 GetPatchWidthVector(const Bezier & patch);

	static float GetPatchRadius(const Bezier & patch);

	static void TrimPatch(Bezier & patch, float trimleft_front, float trimright_front, float trimleft_back, float trimright_back);

	static float GetHorizontalDistanceAlongPatch(const Bezier & patch, Vec3 carposition);

	static float RampBetween(float val, float startat, float endat);

#ifdef VISUALIZE_AI_DEBUG
	VertexArray brakeshape;
	VertexArray steershape;
	VertexArray avoidanceshape;
	std::vector <Bezier> brakelook;
	std::vector <Bezier> steerlook;
	SceneNode::DrawableHandle brakedraw;
	SceneNode::DrawableHandle steerdraw;
	SceneNode::DrawableHandle avoidancedraw;

	static void ConfigureDrawable(SceneNode::DrawableHandle & ref, SceneNode & topnode, float r, float g, float b);

	static void AddLinePoint(VertexArray & va, const Vec3 & p);
#endif
};

#endif // _AI_CAR_STANDARD_H
