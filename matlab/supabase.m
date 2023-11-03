classdef supabase
    properties
        SB_HOST {string}
        SB_KEY {string}
    end
    
    methods
        function obj = supabase(host, key)
            obj.SB_HOST = host;
            obj.SB_KEY = key;
        end

        function [rg07, filters] = get_all_table_entries(obj, table)
            % get request
            uri = strcat(obj.SB_HOST, '/rest/v1/', table, '?', '&select=*');
    
            options = weboptions(...
                'HeaderFields', [matlab.net.http.HeaderField('apikey', obj.SB_KEY), matlab.net.http.HeaderField('Authorization', strcat('Bearer ', obj.SB_KEY)) ]...
            );
        
            % extract table
            rg07 = webread(uri, options);
            
            % extract test days
            millis2days = string(transpose(datetime({rg07(:).millis},'InputFormat','yyyy-MM-dd''T''HH:mm:ss.SSSSSS', 'Format', 'yyyy-MM-dd')));
            days = unique(millis2days, 'rows');
            [rg07.('day')] = millis2days{:};

            filters = [];
            for i = 1:length(days)
                tests = rg07(strcmp({rg07.('day')}, days(i)));
                filter = {days(i), unique(string({tests.('stest')}))};
                filters = [filters; filter];
            end
        end
    end
end