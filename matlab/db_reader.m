clear all
close all

import supabase.*

sb_host = "http://204.216.214.158:8000";
sb_key = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyAgCiAgICAicm9sZSI6ICJhbm9uIiwKICAgICJpc3MiOiAic3VwYWJhc2UtZGVtbyIsCiAgICAiaWF0IjogMTY0MTc2OTIwMCwKICAgICJleHAiOiAxNzk5NTM1NjAwCn0.dc_X5iR_VP_qT0zsiyj_I_OZ2T9FtRU2BBNWN8Bu4GE";
sb_table = "test";

sup = supabase(sb_host, sb_key);
[table, filters] = get_all_table_entries(sup, sb_table);

title = populateGUI(sup,table,filters);

%% AREA DI CALCOLO
% Qui si possono fare tutte le operazioni del caso accedendo ai dati
% semplicemente con il nome della colonna:
%   Ad esempio per accedere alla colonna total_power si può fare: {table(:).total_power}
% I nomi delle colonne si possono vedere aprendo la variabile table dalla
% workspace



%%

function title = populateGUI(sup,table,filters)
    uifig = uifigure('Position',[200,100,1000,720], 'Name', 'log_reader');

    configPanel = uipanel(uifig,"Title","Database");
    configPanel.Position = [0, 655, 1000, 65];
    gridLayout = uigridlayout(configPanel,[1,4]);

    fieldsPanel = uipanel(uifig,"Title","Fields");
    fieldsPanel.Position = [0, 0, 1000, 655];
    
    dayTests = filters(1,2);

    dayDropdown = uidropdown(gridLayout, "Items", string(filters(:,1)), "ItemsData", (1:length(string(filters(:,1)))));
    testDropdown = uidropdown(gridLayout, "Items", dayTests{1}, "ItemsData", (1:length(dayTests{1})));
    loadTestInfosButton = uibutton(gridLayout);
    loadTestButton = uibutton(gridLayout);

    dayDropdown.ValueChangedFcn = @(src,event)updateTests(src,event,filters,testDropdown);

    loadTestInfosButton.Text = 'Load test info';
    loadTestInfosButton.ButtonPushedFcn = @(src,event)loadTestInfos(src,event,sup,dayDropdown,testDropdown);

    loadTestButton.Text = 'Load test';
    loadTestButton.ButtonPushedFcn = @(src,event)loadTest(src,event,table,fieldsPanel,testDropdown);

    loadTest(0, 0, table,fieldsPanel,testDropdown);
    fieldsPanel.Enable = false;
    
    title = dayDropdown.Items(dayDropdown.Value);
end

function updateTests(src,event,filters,testD)
    dayTests = filters(src.Value,2);
    testD.Items = dayTests{1};
    testD.ItemsData = (1:length(dayTests{1}));
end

function loadTest(scr,event,table,fieldsP,testD)
    fieldsP.Enable = true;

    t = table(strcmp(string({table.('stest')}), string(testD.Items(testD.Value))));
    t = rmfield(t,{'id','stest','day'});

    gridLayout = uigridlayout(fieldsP,[15, 7]);

    t_field = fieldnames(t);
    n_fieldname = 1;

    checkBoxes = [];
    for i = 1:7
        for j = 2:15
            if strcmp(t_field(n_fieldname), "millis")
                break;
            end

            newCheck = uicheckbox(gridLayout);
            newCheck.Layout.Row = j;
            newCheck.Layout.Column = i;
            newCheck.Text = t_field(n_fieldname);

            checkBoxes = [checkBoxes; newCheck];
            n_fieldname = n_fieldname + 1;
        end
    end

    showInfos = uibutton(gridLayout);
    showInfos.Text = 'Infos on test';

    actionButton = uibutton(gridLayout);
    actionButton.Text = 'Show Figure';
    actionButton.ButtonPushedFcn = @(btn,event) plotButtonPushed(btn,checkBoxes,t);
    actionButton.Layout.Row = 1;
    actionButton.Layout.Column = 2;
    
    actionButton = uibutton(gridLayout);
    actionButton.Text = 'Close Figures';
    actionButton.ButtonPushedFcn = @(btn,event) closeFigButtonPushed(btn);
    actionButton.Layout.Row = 1;
    actionButton.Layout.Column = 3;
    
    actionButton = uibutton(gridLayout);
    actionButton.Text = 'Show FFT';
    actionButton.ButtonPushedFcn = @(btn,event) fftButtonPushed(btn,checkBoxes,t);
    actionButton.Layout.Row = 1;
    actionButton.Layout.Column = 4;
    
    actionButton = uibutton(gridLayout);
    actionButton.Text = 'Real Time Figure';
    actionButton.ButtonPushedFcn = @(btn,event) animButtonPushed(btn,checkBoxes,t);
    actionButton.Layout.Row = 1;
    actionButton.Layout.Column = 5;
    
    actionButton = uibutton(gridLayout);
    actionButton.Text = 'Clear Selection';
    actionButton.ButtonPushedFcn = @(btn,event) clearButtonPushed(btn,checkBoxes);
    actionButton.Layout.Row = 1;
    actionButton.Layout.Column = 6;
end

function loadTestInfos(src,event,sup,dayDropdown,testDropdown)
    test_info = get_all_test_infos(sup,testDropdown.Value);

    description = string(test_info.description);
    try
        description = eval(description);
    catch
    end

    uifig = uifigure('Position',[1220,420,200,400], 'Name', strcat(string(dayDropdown.Value), ' - ', string(testDropdown.Value)));
    textArea = uitextarea(uifig, 'Value', description, 'Position', [5 50 190 345]);
    updateButton = uibutton(uifig, 'Text', 'Aggiorna', 'Position', [5, 5, 190, 40]);

    updateButton.ButtonPushedFcn = @(btn,event) updateTestInformation(btn,event,uifig,sup,string(testDropdown.Value),string(textArea.Value));
end

function updateTestInformation(btn,event,figure,sup,dropDownValue,textAreaValue)
    update_test_info(sup,dropDownValue,textAreaValue);
    close(figure);
end

function plotButtonPushed(btn,checkBoxes,table)
    t = table;

    millis = double(string(posixtime(datetime({t(:).millis},'InputFormat','yyyy-MM-dd''T''HH:mm:ss.SSSSSS'))));
    t = rmfield(t,{'millis'});

    millis = millis - millis(1);

    fn = fieldnames(t);
    figure();
    hold on;
    grid on;

    titles = [];
    for i = 1:length(fn)
        if checkBoxes(i).Value
            titles = [titles, string(fn(i))];
            plot(millis, double(string({t(:).(string(fn(i)))})));
        end
    end
    legend(titles);
end

function closeFigButtonPushed(btn)
    close all;
end

function fftButtonPushed(btn,checkBoxes,table)
    t = table;

    millis = double(string(posixtime(datetime({t(:).millis},'InputFormat','yyyy-MM-dd''T''HH:mm:ss.SSSSSS'))));
    t = rmfield(t,{'millis'});

    millis = millis - millis(1);

    L = length(millis);
    Fs = 1/mean(diff(millis));
    f = Fs * (0:(L/2))/L;

    fn = fieldnames(t);
    figure();
    hold on;
    grid on;

    titles = [];
    for i = 1:length(fn)
        if checkBoxes(i).Value
            titles = [titles, string(fn(i))];

            fftvalues = fft(double(string({t(:).(string(fn(i)))})));
            fftvalues = abs(fftvalues/L);
            fftvalues = fftvalues(1:L/2+1);
            fftvalues(2:end-1) = 2 * fftvalues(2:end-1);

            plot(f,fftvalues);
        end
    end
    legend(titles);
end

function animButtonPushed(btn,checkBoxes,table)
    t = table;

    millis = double(string(posixtime(datetime({t(:).millis},'InputFormat','yyyy-MM-dd''T''HH:mm:ss.SSSSSS'))));
    t = rmfield(t,{'millis'});

    millis = millis - millis(1);

    fn = fieldnames(t);
    figure();

    titles = [];

    for time = 1:length(millis)-1
        for i = 1:length(fn)
            if checkBoxes(i).Value
                if time == 1
                    titles = [titles, string(fn(i))];
                end
                value = double(string({t(:).(string(fn(i)))}));
                plot(millis(1:time), value(1:time));
            end
        end

        grid on;
        legend(titles);
        pause((millis(time+1) - millis(time)));
    end
end

function clearButtonPushed(btn,checkBoxes)
    for i = 1:length(checkBoxes)
        checkBoxes(i).Value = 0;
    end
end