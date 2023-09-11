all_fig = findall(0, 'type', 'figure');
close(all_fig)
close all
clear all
%% IMPORT DATA

global sb_host;
global sb_key;

% Database link and password
sb_host = "https://wvjiwtbxvehgbdskepdn.supabase.co";
sb_key = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Ind2aml3dGJ4dmVoZ2Jkc2tlcGRuIiwicm9sZSI6ImFub24iLCJpYXQiOjE2OTM4MjEzNzMsImV4cCI6MjAwOTM5NzM3M30.-QJcyNwn_4Dtkvohg-g3GJPb0jCoePxB1MEH7tzFbbo";

% Extract logs
filenames = unique(struct2cell(get_column(sb_host, sb_key, "ecu", 'log_name')), 'rows');
% Extract fieldnames
fields = fieldnames(get_log(sb_host, sb_key, "ecu", 'log_name=eq.STEST0', '*'));
fields = fields(3:end);

% Create GUI
[chk,title] = populateGUI(fields,filenames);

function col = get_column(url, key, table, column)
    params = {'select' column};
    baseURI = strcat(url, '/rest/v1/', table);
    uri = strcat(baseURI, '?', char(join(join(params, '='), '&')));

    options = weboptions(...
        'HeaderFields', [matlab.net.http.HeaderField('apikey', key), matlab.net.http.HeaderField('Authorization', strcat('Bearer ', key)) ]...
    );

    col = webread(uri, options);
end

function log = get_log(url, key, table, filter, selectedRow)
    uri = strcat(url, '/rest/v1/', table, '?', filter, '&select=', selectedRow);
    
    options = weboptions(...
        'HeaderFields', [matlab.net.http.HeaderField('apikey', key), matlab.net.http.HeaderField('Authorization', strcat('Bearer ', key)) ]...
    );

    log = webread(uri, options);
end

function [chk,title] = populateGUI(fields,filenames)
    uifig = uifigure('Position',[200,100,1000,720], 'Name', 'log_reader');
    gl = uigridlayout(uifig,[ceil(length(fields)/5)+2,2]);

    dd = uidropdown(gl, "Items", filenames);
    title = dd.Value;
    
    for col = 1:4
        for i = (ceil(length(fields)/5) * (col - 1) + 1):(ceil(length(fields)/5) * col)
            if i <= length(fields)
                chk(i) = uicheckbox(gl);
                chk(i).Layout.Row = i - (ceil(length(fields)/5) * (col-1)) + 2;
                chk(i).Layout.Column = col;
                chk(i).Text = fields(i);
            end
        end
    end

    btn = uibutton(gl);
    btn.Text = 'Show Figure';
    btn.ButtonPushedFcn = @(btn,event) plotButtonPushed(btn,chk,dd,fields);
    btn.Layout.Row = 1;
    btn.Layout.Column = 2;
    
    btn = uibutton(gl);
    btn.Text = 'Show FFT';
    btn.ButtonPushedFcn = @(btn,event) fftButtonPushed(btn,chk,dd,fields);
    btn.Layout.Row = 1;
    btn.Layout.Column = 3;
    
    btn = uibutton(gl);
    btn.Text = 'Clear Selection';
    btn.ButtonPushedFcn = @(btn,event) clearButtonPushed(btn,chk,dpd);
    btn.Layout.Row = 1;
    btn.Layout.Column = 4;

    btn = uibutton(gl);
    btn.Text = 'Build operation';
    btn.ButtonPushedFcn = @(btn,event) buildOperation(btn);
    btn.Layout.Row = 1;
    btn.Layout.Column = 5;
end

function plotButtonPushed(btn,chk,dd,fields)
    global sb_host;
    global sb_key;
    data = get_log(sb_host, sb_key, "ecu", strcat('log_name=eq.', char(dd.Value)), '*');

    % extract timestamp
    t = [data(:).timestamp]*1e-2;

    figure
    grid on
    hold on

    % legend
    titles = {};

    M = 0;
    m = 1e12;
    for i = 1:length(fields)
        % check selected
        if (chk(i).Value == 1)
            values = [data(:).(char(chk(i).Text))];
            plot(t,values);
            titles{end+1} = fields{i};
            mindata = min(values);
            maxdata = max(values);
            if (mindata < m) 
                m = mindata;
            end
            if (maxdata > M) 
                M = maxdata;
            end
        end
    end
    legend(titles);
    xlabel('Time [s]');
    xlim([0, t(end)]);
    if (m ~= M)
        ylim([m*.9 M*1.1]);
    end
end

function clearButtonPushed(btn,chk,dpd)
    for i = 1:length(chk)
        chk(i).Value = 0;
    end
    dpd.Value = 'None';
end

function fftButtonPushed(btn,chk,dd,fields)
    global sb_host;
    global sb_key;
    data = get_log(sb_host, sb_key, "ecu", strcat('log_name=eq.', char(dd.Value)), '*');

    % extract timestamp
    t = [data(:).timestamp]*1e-2;

    L = length(t);
    Fs = 1/mean(diff(t));
    f = Fs * (0:(L/2))/L;
    
    figure
    grid on
    hold on
    titles = {};
    M = 0;
    m = 1e12;
    for i = 1:length(fields)
        if (chk(i).Value == 1)
            values = [data(:).(char(chk(i).Text))];
            fftvalues = fft(values);
            fftvalues = abs(fftvalues/L);
            fftvalues = fftvalues(1:L/2+1);
            fftvalues(2:end-1) = 2 * fftvalues(2:end-1);
            plot(f,fftvalues);
            titles{end+1} = fields{i};
            mindata = min(fftvalues);
            maxdata = max(fftvalues);
            if (mindata < m) 
                m = mindata;
            end
            if (maxdata > M) 
                M = maxdata;
            end
        end
    end
    legend(titles);
    xlabel('Frequency [Hz]');
    xlim([0, f(end)]);
    if (m ~= M)
        ylim([m*.9 M*1.1]);
    end
end