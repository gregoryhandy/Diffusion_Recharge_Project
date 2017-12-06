clear; close all; clc;

% loop through all tau_r values
for tau_r_count = 1:3
    % define the tau_r value accordingly
    if tau_r_count == 1
        tau_r =1;
    elseif tau_r_count == 2
        tau_r =0.1;
    else
        tau_r =0.02;
    end
    
    n = [100 250 500 1000 5000 10000];

    %% Upper Bound
    lambda_0 = (pi/2)^2;
    tau_e = 1/lambda_0; % D = 1 for these simulations
    
    C =  get_optimal_c;
    
    smooth_n = [min(n):1:max(n)];

    T = tau_r/tau_e;
    
    x = log(smooth_n*T*C);
    positive_part = (abs(x)+x)/2;
    ub_1 = 1./(T).*positive_part+1+min(smooth_n*C,1./T);
    ub_2 = smooth_n*.5;
    
    ub = min(ub_1,ub_2);
    
    %% Read in simulation data for each value of n
    mean_num_act = zeros(length(n),1);
    num_trials = zeros(length(n),1);
    for i = 1:length(n)
        str1 = sprintf('./N_raw_data/');
        str2 = sprintf('/num_activated_%d_%.2f.dat',n(i),tau_r);
        temp_name = strcat(str1,str2);
        fid = fopen(temp_name,'r');
        datacell = textscan(fid, '%d %f', 'headerlines', 1);
        fclose all;
        
        num_activated = datacell{1};
        dur_activated = datacell{2};
        sim_trials = length(num_activated);
        
        mean_num_act(i) =  mean(num_activated);
        num_trials(i) = length(num_activated);
    end

    %% Graph a curve for each value of tau_r
    if tau_r_count==1
        h1 = plot(smooth_n, ub,'-','color',[1 .5 0],'linewidth',1.5,'markersize',20);
        hold on;
        plot(n,mean_num_act,'.--','color',[1 .5 0],'linewidth',1.5,'markersize',20);
        T1 = T;
    elseif tau_r_count ==2
        h2 = plot(smooth_n, ub,'-','color',[0 .5 0],'linewidth',1.5,'markersize',20);
        hold on;
        plot(n,mean_num_act,'.--','color',[0 .5 0],'linewidth',1.5,'markersize',20);
        T2 = T;
    else
        h3 = plot(smooth_n, ub,'-','color','black','linewidth',1.5,'markersize',20);
        hold on;
        plot(n,mean_num_act,'.--','color','black','linewidth',1.5,'markersize',20);
        T3 = T;
    end
    
    set(gca,'fontsize',16)
    xlabel('Number of Initial Molecules (n)')
    ylabel('Expected Number of Captures  {\bfE}(N)')

end

%% Define the legend
T1_legend = sprintf('T=%.4f',T1);
T2_legend = sprintf('T=%.4f',T2);
T3_legend = sprintf('T=%.4f',T3);
legend([h3 h2 h1],{T3_legend,T2_legend,T1_legend});


