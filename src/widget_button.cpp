#include "widget_button.h"

#include <cassert>

WIDGET_BUTTON::WIDGET_BUTTON() : 
	cancel(true),
	screenhwratio(1)
{
	// ctor
}

WIDGET * WIDGET_BUTTON::clone() const
{
	return new WIDGET_BUTTON(*this);
}

void WIDGET_BUTTON::SetAlpha(SCENENODE & scene, float newalpha)
{
	label.SetAlpha(scene, newalpha);
	image_up.SetAlpha(scene, newalpha);
	image_down.SetAlpha(scene, newalpha);
	image_selected.SetAlpha(scene, newalpha);
}

void WIDGET_BUTTON::SetVisible(SCENENODE & scene, bool newvis)
{
	label.SetVisible(scene, newvis);
	if (state == UP)
		image_up.SetVisible(scene, newvis);
	else if (state == DOWN)
		image_down.SetVisible(scene, newvis);
	else if (state == SELECTED)
		image_selected.SetVisible(scene, newvis);
}

std::string WIDGET_BUTTON::GetAction() const
{
	return active_action;
}

std::string WIDGET_BUTTON::GetDescription() const
{
	return description;
}

void WIDGET_BUTTON::SetDescription(const std::string & newdesc)
{
	description = newdesc;
}

bool WIDGET_BUTTON::GetCancel() const
{
	return cancel;
}

bool WIDGET_BUTTON::ProcessInput(SCENENODE & scene, float cursorx, float cursory, bool cursordown, bool cursorjustup)
{
	active_action.clear();
	
	if (cursorx < image_up.GetCorner2()[0]+h/(screenhwratio*3.0) && cursorx > image_up.GetCorner1()[0]-h/(screenhwratio*3.0) &&
		cursory < image_up.GetCorner2()[1] && cursory > image_up.GetCorner1()[1])
	{
		if (cursordown && state != DOWN)
		{
			state = DOWN;
			image_down.SetVisible(scene, true);
			image_up.SetVisible(scene, false);
			image_selected.SetVisible(scene, false);
			
			//std::cout << "depress" << std::endl;
		}
		else if (!cursordown && state != SELECTED)
		{
			state = SELECTED;
			image_down.SetVisible(scene, false);
			image_up.SetVisible(scene, false);
			image_selected.SetVisible(scene, true);
		}
		
		//std::cout << "hover" << std::endl << std::endl;
		
		if (cursorjustup)
		{
			//take some action
			active_action = action;
		}
		
		return true;
	}
	else
	{
		if (state != UP)
		{
			state = UP;
			image_down.SetVisible(scene, false);
			image_up.SetVisible(scene, true);
			image_selected.SetVisible(scene, false);
		}
		
		//std::cout << image_up.GetCorner1() << " x " << image_up.GetCorner2() << cursorx << "," << cursory << std::endl << std::endl;
		return false;
	}
}

void WIDGET_BUTTON::SetCancel(bool newcancel)
{
	cancel = newcancel;
}

void WIDGET_BUTTON::SetAction(const std::string & newaction)
{
	action = newaction;
}

void WIDGET_BUTTON::SetupDrawable(
	SCENENODE & scene,
	std::tr1::shared_ptr<TEXTURE> teximage_up,
	std::tr1::shared_ptr<TEXTURE> teximage_down,
	std::tr1::shared_ptr<TEXTURE> teximage_selected,
	const FONT * font,
	const std::string & text,
	const float centerx,
	const float centery,
	const float scalex,
	const float scaley,
	const float r,
	const float g,
	const float b)
{
	assert(teximage_up);
	assert(teximage_down);
	assert(teximage_selected);
	assert(font);
	//assert(!text.empty());
	
	float w = label.GetWidth(font, text, scalex);//*(scaley/scalex);
	//h = 0.06*scaley*4.0;
	h = 0.06*scaley*4.0;
	
	screenhwratio = scaley/scalex;
	
	//float x = centerx - w*0.5;
	float y = centery;
	
	label.SetupDrawable(scene, font, text, centerx, y+0.007, scalex, scaley, r, g, b, 2);
	image_up.SetupDrawable(scene, teximage_up, centerx, centery, w, h, 1, true, scaley/scalex);
	image_down.SetupDrawable(scene, teximage_down, centerx, centery, w, h, 1, true, scaley/scalex);
	image_selected.SetupDrawable(scene, teximage_selected, centerx, centery, w, h, 1, true, scaley/scalex);
	image_down.SetVisible(scene, false);
	image_selected.SetVisible(scene, false);
	state = UP;
}