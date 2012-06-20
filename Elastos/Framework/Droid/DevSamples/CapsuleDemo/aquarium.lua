----------------------------------------------------------------------------
-- Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
----------------------------------------------------------------------------

require("math");
require("os");

local SCREEN_WIDTH,SCREEN_HEIGHT = 320,480;

local WIN_WIDTH,WIN_HEIGHT = 320,480;
local WIN_LEFT = 0;
local WIN_TOP = 0;
local bubble_width = 15;
local fish_width,fish_height = 30,22;
local bubble_num = 5;

Window:SetWidth(320);
Window:SetHeight(480);

bgPanel:SetPosition(0,0);
bgPanel:SetWidth(WIN_WIDTH);
bgPanel:SetHeight(WIN_HEIGHT);
bgPanel:LoadImageFromFile("Aquarium/images/aquarium.png");

bgPanel:SetImageAlign(Controls.ImageAlign.Stretch);

touch:SetPosition(0,0);
touch:SetWidth(WIN_WIDTH);
touch:SetHeight(WIN_HEIGHT);

bubble = {bubble1,bubble2,bubble3,bubble4,bubble5};
bubble_UserData = {0, 0, 0, 0, 0};

local bubbleWidth = WIN_WIDTH/6;
for i=1, bubble_num do
    bubble[i]:SetPosition((bubbleWidth*i), WIN_HEIGHT+math.random(1,1000));
end

fish1:SetPosition((WIN_WIDTH/2),(WIN_HEIGHT/2));
fish2:SetPosition((WIN_WIDTH/4),(WIN_HEIGHT/4));


for i=1,bubble_num do
    bubble[i]:LoadImageFromFile("Aquarium/images/bubble.png");
    bubble[i]:SetWidth(bubble_width);
    bubble[i]:SetHeight(bubble_width);
    bubble[i]:SetImageAlign(Controls.ImageAlign.Stretch);
end

local bubble_count_in_screen = 0;

function on_timer()

    fish1_relocate();
    fish2_relocate();

    for k=1,bubble_num do

        if bubble_UserData[k] ~= 0 then
            bubble_relocate(k);
        end
        
        if bubble_UserData[k] == 0 and
            bubble_count_in_screen < bubble_num then
            bubble_UserData[k] = k;       -- bubble index
            bubble_count_in_screen = bubble_count_in_screen + 1;
        end
    end

end


local offset_top = 12;
local offset_left = 2;

function bubble_relocate(i)

    bubble_left = bubble[i]:GetX();
    bubble_top = bubble[i]:GetY();

    bubble_left = bubble_left + offset_left;
    if bubble_left < 0 or
            bubble_left > (WIN_WIDTH - bubble_width) then
        bubble_left = bubble_left - offset_left;
    end
    offset_left = -offset_left;       -- shivering effect

    bubble_top = bubble_top - offset_top;
    if bubble_top <= 0 then
        bubble_UserData[i] = 0;
        bubble_count_in_screen = bubble_count_in_screen - 1;
        bubble_top = bubble_top + WIN_HEIGHT + bubble_width + math.random(1,1000);
    end
    bubble[i]:SetPosition(bubble_left, bubble_top);
end


local random_tweak = 7;
local fish1_offset_x = 1;
local fish1_offset_y = 1;
local fish2_offset_x = 1;
local fish2_offset_y = 1;
local time1 = 0;
local time2 = 0;
local flag1 = 0;
local flag2 = 0;

function fish1_relocate()
    fish1_left = fish1:GetX();
    fish1_top = fish1:GetY();

    if time1>15 then
        flag1 = 0;
        time1 = 0;
        fish1_offset_x = 1;
        math.randomseed(os.time() - random_tweak);
        math.randomseed(math.random(10000,40000));
        random_tweak = math.random(1,1000) + random_tweak;
        if random_tweak>20000 then
            random_tweak = 7;
        end
        if math.random()>0.5 then
            fish1_offset_x = -fish1_offset_x;
        end
        if math.random()>0.75 then
            fish1_offset_y = -fish1_offset_y;
        elseif math.random()<0.5 then
            flag1 = 1;       ---move horizontal
        end
    end

    time1 = time1 + 1;

    fish1_top = fish1_top + fish1_offset_y;

    fish1_left = fish1_left + fish1_offset_x;

    if fish1_top < 0 or fish1_top > WIN_HEIGHT - fish_height - 5 then
        fish1_top = fish1_top - fish1_offset_y;
    end

    if fish1_left < 0 or fish1_left > WIN_WIDTH - fish_width then
        fish1_left = fish1_left - fish1_offset_x;
    end

    fish1:SetPosition(fish1_left, fish1_top);

    if fish1_offset_x >= 0 then
        fish1:LoadImageFromFile("Aquarium/images/fishR.png");
    else
        fish1:LoadImageFromFile("Aquarium/images/fishL.png");      -- reverse image
    end
end

function fish2_relocate()
    fish2_left = fish2:GetX();
    fish2_top = fish2:GetY();

    if time2>20 then
        flag2 = 0;
        time2 = 0;
        fish2_offset_x = 1;
        math.randomseed(os.time()-random_tweak);
        math.randomseed(math.random(100000,400000));
        random_tweak = math.random(1,1000)+random_tweak;
        if random_tweak>20000 then
            random_tweak = 7;
        end
        if math.random()>0.5 then
            fish2_offset_x = -fish2_offset_x;
        end
        if math.random()>0.66 then
            fish2_offset_y = -fish2_offset_y;
        elseif math.random()<0.4 then
            flag2 = 1;
        end
    end

    time2 = time2+1;

    fish2_top = fish2_top + fish2_offset_y;

    fish2_left = fish2_left + fish2_offset_x;

    if fish2_top < 0 or fish2_top > WIN_HEIGHT - fish_height - 5 then
        fish2_top = fish2_top - fish2_offset_y;
    end

    if fish2_left < 0 or fish2_left > WIN_WIDTH - fish_width then
        fish2_left = fish2_left - fish2_offset_x;
    end

    fish2:SetPosition(fish2_left, fish2_top);

    if fish2_offset_x >= 0 then
        fish2:LoadImageFromFile("Aquarium/images/fishR.png");
    else
        fish2:LoadImageFromFile("Aquarium/images/fishL.png");      -- reverse image
    end
end

local safeDistance = fish_width;
local fish_speed = 18;

function OnClick()
    local x = Event.x
    local y = Event.y
    local left = fish1:GetX();
    local top = fish1:GetY();
    distance1 = (x - (left + fish_width/2))^2 +
                        (y - (top + fish_height/2))^2;

    left = fish2:GetX();
    top = fish2:GetY();
    distance2 = (x - (left + fish_width/2))^2 +
                        (y - (top + fish_height/2))^2;

    if distance1 < safeDistance^2 then
        time1 = 0;              --fish1 runaway
        left = fish1:GetX();
        if x > (WIN_WIDTH - fish_width) or (x > fish_width and
                        x > (left + fish_width/2)) then
            fish1_offset_x = -fish_speed;
        else
            fish1_offset_x = fish_speed;
        end
    end
    if distance2 < safeDistance^2 then
        time2 = 0;               --fish2 runaway
        left = fish2:GetX();
        if x > (WIN_WIDTH - fish_width) or (x > fish_width and
                        x > (left + fish_width/2)) then
            fish2_offset_x = -fish_speed;
        else
            fish2_offset_x = fish_speed;
        end
    end
end

function OnClickExit()
    clock:Stop();
    Window:Close();
end

touch.StylusPressed = OnClick
touch.StylusHeld = OnClickExit

clock = Grafix.GrafixTimer:CreateObject({interval = 600});
clock.Timer = on_timer;
clock:Start();

Window:Show();
