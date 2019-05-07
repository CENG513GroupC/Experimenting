clear all; close all;
clc;

nof_packets = 1001;
% Experiment 1 Specs
exp1_succesfull = [631, 580, 573, 583, 608, 582, 624, 584, 604, 577];
% Experiment 2 Specs
exp2_succesfull = [764, 776, 781, 774, 794, 826, 785, 814, 795, 782];
% Experiment 3 Specs
exp3_succesfull = [792, 776, 762, 789, 788, 769, 801, 772, 812, 792];


boxplot([exp1_succesfull', exp2_succesfull', exp3_succesfull']);
ylabel('Packet Rates (%)')
title('Packet Rate Experiments')
grid on;
ax = gca;
set(gca, 'XTickLabel', {'Experiment 1', 'Experiment 2', 'Experiment 3'});
ax.GridColor = [0 .1 .1];
ax.GridLineStyle = '--';
ax.GridAlpha = 0.2;
ax.Layer = 'top';

disp(mean(exp1_succesfull));
disp(var(exp1_succesfull));

disp(mean(exp2_succesfull));
disp(var(exp2_succesfull));

disp(mean(exp3_succesfull));
disp(var(exp3_succesfull));
