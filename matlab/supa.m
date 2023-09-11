clear all;
close all;

sb_host = "https://wvjiwtbxvehgbdskepdn.supabase.co";
sb_key = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Ind2aml3dGJ4dmVoZ2Jkc2tlcGRuIiwicm9sZSI6ImFub24iLCJpYXQiOjE2OTM4MjEzNzMsImV4cCI6MjAwOTM5NzM3M30.-QJcyNwn_4Dtkvohg-g3GJPb0jCoePxB1MEH7tzFbbo";

at = ""; %database_login(sb_host, sb_key, "ecu@raceup.it", "Centralina");

avaiable_logs = unique(struct2cell(get_column(at, sb_host, sb_key, "ecu", 'log_name')), 'rows');
avaiable_cols = fieldnames(get_log("", sb_host, sb_key, "ecu", 'log_name=eq.STEST0', '*'));
avaiable_cols = avaiable_cols(2:end);

%logs = get_log(at, sb_host, sb_key, "ecu", strcat('log_name=eq.', char(avaiable_logs(2))), '*');

function login = database_login(url, key, email, pass)
    params = {'grant_type' 'password'};

    baseURI = strcat(url, '/auth/v1/token');
    uri = strcat(baseURI, '?', char(join(join(params, '='), '&')));
    
    body = struct(...
        'email', email,...
        'password', pass...
    );

    options = weboptions(...
        'MediaType', 'application/json',...
        'HeaderFields', matlab.net.http.HeaderField('apikey', key)...
    );

    response = webwrite(uri, body, options);

    login = response.access_token;
end

function col = get_column(access_token, url, key, table, column)
    params = {'select' column};
    baseURI = strcat(url, '/rest/v1/', table);
    uri = strcat(baseURI, '?', char(join(join(params, '='), '&')));

    options = weboptions(...
        'HeaderFields', [matlab.net.http.HeaderField('apikey', key), matlab.net.http.HeaderField('Authorization', strcat('Bearer ', key)) ]...
    );

    col = webread(uri, options);
end

function log = get_log(access_token, url, key, table, filter, selectedRow)
    uri = strcat(url, '/rest/v1/', table, '?', filter, '&select=', selectedRow);
    
    options = weboptions(...
        'HeaderFields', [matlab.net.http.HeaderField('apikey', key), matlab.net.http.HeaderField('Authorization', strcat('Bearer ', key)) ]...
    );

    log = webread(uri, options);
end

function gui = create_gui(logs, cols)
    
end