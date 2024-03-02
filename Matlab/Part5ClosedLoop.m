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
% latitude = 36.9741;   % in degrees
% longitude = -122.0308; % in degrees
% altitude = 259.08;          % in meters (assuming sea level)
gravity = 9.81;       % in meters per second squared
nT = 23194.3/1000;           % nanoTeslas from microTeslas

% Inertial gravity vector points opposite to the NED position vector
accelInertial = [gravity; 0; 0];

% Define a unit vector pointing in the direction of magnetic north
magInertial = [0; 0; nT];

% Step 2: Extract true Euler angles and bias
yaw_true = deg2rad(Eul_true(:, 1));
pitch_true = deg2rad(Eul_true(:, 2));
roll_true = deg2rad(Eul_true(:, 3));

yaw_100 = wGyro(1:50,1);
pitch_100 = wGyro(1:50,2);
roll_100 = wGyro(1:50,3);

yaw_bias = mean(yaw_100);
pitch_bias = mean(pitch_100);
roll_bias = mean(roll_100);


% Step 3: Initialize DCM from true Euler angles
% R_0 = [(cos(pitch_true(1))*cos(yaw_true(1))), cos(pitch_true(1))*sin(yaw_true(1)), -sin(pitch_true(1));
%         (sin(roll_true(1))*sin(pitch_true(1))*cos(yaw_true(1)))-(cos(roll_true(1))*sin(yaw_true(1))), (sin(roll_true(1))*sin(pitch_true(1))*sin(yaw_true(1))) + (cos(roll_true(1))*cos(yaw_true(1))), sin(roll_true(1))*cos(pitch_true(1));
%         (cos(roll_true(1))*sin(pitch_true(1))*cos(yaw_true(1)))+ (sin(roll_true(1))*sin(yaw_true(1))), (cos(roll_true(1))*sin(pitch_true(1))*sin(yaw_true(1))) - (sin(roll_true(1))*cos(yaw_true(1))), cos(roll_true(1))*cos(pitch_true(1))];

R_0 = [1, 0, 0;
        0, 1, 0;
        0, 0, 1];

% Step 4: Integrate gyroscope output using forward integration and matrix exponential form
pitches =[];
yaws = [];
rolls = [];
Bminus = [yaw_bias ; pitch_bias; roll_bias];
%Bminus = [0; 0; 0];
for i = 1:length(pitch_true)
    
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

PrettyPlotAttitudeData(dT,Acc,Mag,wGyro,Eul_true)

subplot(3, 1, 1);
plot(rad2deg(yaw_true), 'b');
title('Yaw Error (Exponential Integration)');
hold on;
subplot(3, 1, 1);
plot(yaws, 'r');
title('Yaw Error (Exponential Integration)');

subplot(3,1, 2);
ylabel('Degrees of Difference');
plot(rad2deg(pitch_true), 'b');
title('Pitch Error (Exponential Integration)');
hold on;
subplot(3, 1, 2);
ylabel('Degrees of Difference');
plot(pitches, 'r');
title('Pitch Error (Exponential Integration)');


subplot(3, 1, 3);
plot(rad2deg(roll_true), 'b');
xlabel('time (dt)');
title('Roll Error (Exponential Integration)');
hold on;
subplot(3, 1, 3);
plot(rolls, 'r');
xlabel('time (dt)');
title('Roll Error (Exponential Integration)');


