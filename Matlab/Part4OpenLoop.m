%% Part 4.1 Integration with Open-Loop
clc;
clear; 
close all;

% Step 1: Generate trajectory data without noise
dT = 1/50; % Set timestep to match real data (e.g., 50Hz)
noiseFlag = false; % No noise

[Acc, Mag, wGyro, Eul_true] = CreateTrajectoryData(dT, noiseFlag);

% Step 2: Extract true Euler angles
yaw_true = Eul_true(:, 1);
pitch_true = Eul_true(:, 2);
roll_true = Eul_true(:, 3);

% Step 3: Initialize DCM from true Euler angles
R_0 = [(cos(pitch_true(1))*cos(yaw_true(1))), cos(pitch_true(1))*sin(yaw_true(1)), -sin(pitch_true(1));
        (sin(roll_true(1))*sin(pitch_true(1))*cos(yaw_true(1)))-(cos(roll_true(1))*sin(yaw_true(1))), (sin(roll_true(1))*sin(pitch_true(1))*sin(yaw_true(1))) + (cos(roll_true(1))*cos(yaw_true(1))), sin(roll_true(1))*cos(pitch_true(1));
        (cos(roll_true(1))*sin(pitch_true(1))*cos(yaw_true(1)))+ (sin(roll_true(1))*sin(yaw_true(1))), (cos(roll_true(1))*sin(pitch_true(1))*sin(yaw_true(1))) - (sin(roll_true(1))*cos(yaw_true(1))), cos(roll_true(1))*cos(pitch_true(1))];

% Step 4: Integrate gyroscope output using forward integration and matrix exponential form
pitches =[];
yaws = [];
rolls = [];

Rk_1 = IntegrateOpenLoop(R_0, wGyro(1, :), dT);
pitch = -asin(Rk_1(1, 3));
yaw = asin(Rk_1(1,2)/cos(pitch));
roll = asin(Rk_1(2, 3)/cos(pitch));
pitches = [pitches; rad2deg(pitch)];
yaws = [yaws; rad2deg(yaw)];
rolls = [rolls; rad2deg(roll)];
% pitches = [pitches; pitch];
% yaws = [yaws; yaw];
% rolls = [rolls; roll];
for i = 2:length(pitch_true)
    Rk_1 = IntegrateOpenLoop(Rk_1, wGyro(i, :), dT);
    % Step 5: Extract Euler angles from integrated DCMs
    pitch = -asin(Rk_1(1, 3));
    yaw = asin(Rk_1(1,2)/cos(pitch));
    roll = asin(Rk_1(2, 3)/cos(pitch));
    % pitches = [pitches; pitch];
    % yaws = [yaws; yaw];
    % rolls = [rolls; roll];
     pitches = [pitches; rad2deg(pitch)];
     yaws = [yaws; rad2deg(yaw)];
     rolls = [rolls; rad2deg(roll)];

end

% Step 6: Compare calculated Euler angles with true Euler angles and plot errors
figure(1);
subplot(3, 1, 1);
plot(yaw_true - yaws);
title('Yaw Error (Exponential Integration)');
subplot(3, 1, 2);
plot(pitch_true - pitches);
title('Pitch Error (Exponential Integration)');
subplot(3, 1, 3);
plot(roll_true - rolls);
title('Roll Error (Exponential Integration)');

