% Author: Nadia Shabbar
% Date: 2/29/2024

% Part 5, Integration with Open-Loop
clc;
clear; 
close all;

% Step 1: Generate trajectory data without noise
dT = 1/50; % Set timestep to match real data (e.g., 50Hz)
noiseFlag = true; % we want noise

[Acc, Mag, wGyro, Eul_true] = CreateTrajectoryData(dT, noiseFlag);

% Define geographical coordinates of Santa Cruz, CA
latitude = 36.9741;   % in degrees
longitude = -122.0308; % in degrees
altitude = 259.08;          % in meters (assuming sea level)
gravity = 9.81;       % in meters per second squared
nT = 23194.3/1000;           % nanoTeslas

% Inertial gravity vector points opposite to the NED position vector
accelInertial = [gravity; 0; 0];

% Define a unit vector pointing in the direction of magnetic north
magInertial = [nT; 0; 0];

% Step 2: Extract true Euler angles and bias
yaw_true = deg2rad(Eul_true(:, 1));
pitch_true = deg2rad(Eul_true(:, 2));
roll_true = deg2rad(Eul_true(:, 3));



% Step 3: Initialize DCM from true Euler angles
R_0 = [(cos(pitch_true(1))*cos(yaw_true(1))), cos(pitch_true(1))*sin(yaw_true(1)), -sin(pitch_true(1));
        (sin(roll_true(1))*sin(pitch_true(1))*cos(yaw_true(1)))-(cos(roll_true(1))*sin(yaw_true(1))), (sin(roll_true(1))*sin(pitch_true(1))*sin(yaw_true(1))) + (cos(roll_true(1))*cos(yaw_true(1))), sin(roll_true(1))*cos(pitch_true(1));
        (cos(roll_true(1))*sin(pitch_true(1))*cos(yaw_true(1)))+ (sin(roll_true(1))*sin(yaw_true(1))), (cos(roll_true(1))*sin(pitch_true(1))*sin(yaw_true(1))) - (sin(roll_true(1))*cos(yaw_true(1))), cos(roll_true(1))*cos(pitch_true(1))];

% Step 4: Integrate gyroscope output using forward integration and matrix exponential form
pitches =[];
yaws = [];
rolls = [];

for i = 1:length(pitch_true)
    Bminus = [0; 0; 0];
    if i==1
        [Rk_1, g_bias] = IntegrateClosedLoop(R_0, Bminus, deg2rad(wGyro(i, :)./131), Mag(i,:), Acc(i,:), magInertial, accelInertial, dT);
    else
        [Rk_1, g_bias]= IntegrateClosedLoop(Rk_1, g_bias, deg2rad(wGyro(i, :)./131), Mag(i,:), Acc(i,:), magInertial, accelInertial, dT);
    end
    % Step 5: Extract Euler angles from integrated DCMs
    pitch = -asin(Rk_1(1, 3));
    yaw = atan2(Rk_1(1,2),Rk_1(1,1));
    roll = atan2(Rk_1(2,3),Rk_1(3,3));
    pitches = [pitches; rad2deg(pitch)];
    yaws = [yaws; rad2deg(yaw)];
    rolls = [rolls; rad2deg(roll)];
end

subplot(3, 2, 1);
plot(rad2deg(yaw_true));
title('Yaw Error (Exponential Integration)');
subplot(3, 2, 2);

ylabel('Degrees of Difference');
plot(rad2deg(pitch_true));
title('Pitch Error (Exponential Integration)');

subplot(3, 2, 3);
plot(rad2deg(roll_true));
xlabel('time (dt)');
title('Roll Error (Exponential Integration)');

subplot(3, 2, 4);
plot(yaws);
title('Yaw');
subplot(3, 2, 5);

ylabel('Degrees of Difference');
plot(pitches);
title('Pitch');

subplot(3, 2, 6);
plot(rolls);
xlabel('time (dt)');
title('Roll');

% subplot(4, 2, 1);
% plot(Acc);
% xlabel('time (dt)');
% title('ACC');
% 
% 
% 
% subplot(4, 2, 2);
% plot(Mag);
% xlabel('time (dt)');
% title('Mag');


