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
Ro_initial = zeros(3, 3, length(yaw_true));
for i = 1:length(yaw_true)
    Ro_initial(:, :, i) = Rexp(deg2rad([yaw_true(i), pitch_true(i), roll_true(i)]), 1.0);
end

% Step 4: Integrate gyroscope output using forward integration and matrix exponential form
R_forward = zeros(3, 3, length(yaw_true));
R_exp = zeros(3, 3, length(yaw_true));
for i = 1:length(yaw_true)
    if i == 1
        R_forward(:, :, i) = Ro_initial(:, :, i);
        R_exp(:, :, i) = Ro_initial(:, :, i);
    else
        R_forward(:, :, i) = IntegrateOpenLoop(R_forward(:, :, i-1), wGyro(i, :)' * pi / 180, dT);
        R_exp(:, :, i) = Rexp(wGyro(i, :)' * pi / 180, dT) * R_exp(:, :, i-1);
    end
end

% Step 5: Extract Euler angles from integrated DCMs
[yaw_forward, pitch_forward, roll_forward] = extractEulerAngles(R_forward);
[yaw_exp, pitch_exp, roll_exp] = extractEulerAngles(R_exp);

% Step 6: Compare calculated Euler angles with true Euler angles and plot errors
figure(10);
subplot(3, 1, 1);
plot(yaw_true - rad2deg(yaw_forward));
title('Yaw Error (Forward Integration)');
subplot(3, 1, 2);
plot(pitch_true - rad2deg(pitch_forward));
title('Pitch Error (Forward Integration)');
subplot(3, 1, 3);
plot(roll_true - rad2deg(roll_forward));
title('Roll Error (Forward Integration)');

figure(11);
subplot(3, 1, 1);
plot(yaw_true - rad2deg(yaw_exp));
title('Yaw Error (Matrix Exponential Form)');
subplot(3, 1, 2);
plot(pitch_true - rad2deg(pitch_exp));
title('Pitch Error (Matrix Exponential Form)');
subplot(3, 1, 3);
plot(roll_true - rad2deg(roll_exp));
title('Roll Error (Matrix Exponential Form)');

% Step 7: Repeat the process with noisy data
noiseFlag = true; % Add noise

[Acc_noisy, Mag_noisy, wGyro_noisy, Eul_true_noisy] = CreateTrajectoryData(dT, noiseFlag);

% Step 8: Find gyroscope biases and subtract them from subsequent gyroscope readings
gyro_bias = mean(wGyro_noisy(:, 1:3)); % Estimate bias from the first second
wGyro_noisy_corrected = wGyro_noisy - gyro_bias;

% Step 9: Integrate again and compare
R_exp_noisy = zeros(3, 3, length(yaw_true));
for i = 1:length(yaw_true)
    if i == 1
        R_exp_noisy(:, :, i) = Ro_initial(:, :, i);
    else
        R_exp_noisy(:, :, i) = Rexp(wGyro_noisy_corrected(i, :)' * pi / 180, dT) * R_exp_noisy(:, :, i-1);
    end
end

[yaw_exp_noisy, pitch_exp_noisy, roll_exp_noisy] = extractEulerAngles(R_exp_noisy);

% Plot errors for noisy integration
figure(12);
subplot(3, 1, 1);
plot(yaw_true - rad2deg(yaw_exp_noisy));
title('Yaw Error (Noisy Data)');
subplot(3, 1, 2);
plot(pitch_true - rad2deg(pitch_exp_noisy));
title('Pitch Error (Noisy Data)');
subplot(3, 1, 3);
plot(roll_true - rad2deg(roll_exp_noisy));
title('Roll Error (Noisy Data)');

% Function to extract Euler angles from DCM
function [yaw, pitch, roll] = extractEulerAngles(R)
    yaw = zeros(size(R, 3), 1);
    pitch = zeros(size(R, 3), 1);
    roll = zeros(size(R, 3), 1);
    for i = 1:size(R, 3)
        [yaw(i), pitch(i), roll(i)] = dcm2angle(R(:, :, i), 'ZYX');
    end
end

