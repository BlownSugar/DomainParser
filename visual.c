#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> 
#define MAX_WEBSITES 100
#define MAX_LINE 1024

typedef struct {
    char website[MAX_LINE];
    int counter;
} WebsiteCount;

typedef struct {
    char url[MAX_LINE];
    char login[MAX_LINE];
    char password[MAX_LINE];
} Data;

char *trim(char *str) {
    char *end;

    // Trim leading spaces
    while(isspace((unsigned char)*str)) str++;

    // All spaces?
    if(*str == 0) return str;

    // Trim trailing spaces
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Null-terminate
    end[1] = '\0';

    return str;
}

int main() {
    FILE *check = fopen("check.txt", "r");
    FILE *csv = fopen("output.csv", "r");
    FILE *html = fopen("output.html", "w");

    if (!check || !csv || !html) {
        printf("Error opening files.\n");
        return 1;
    }

    // CSS
    fprintf(html, "<html><head><style>");
    fprintf(html, "body { font-family: Arial, sans-serif; margin: 40px; }");
    fprintf(html, "h2 { background-color: #f2f2f2; padding: 10px; }");
    fprintf(html, "table { width: 100%%; border-collapse: collapse; margin-top: 20px; }");
    fprintf(html, "th, td { border: 1px solid #dddddd; padding: 8px; text-align: left; }");
    fprintf(html, "th { background-color: #f2f2f2; }");
    fprintf(html, "</style></head><body>");

    char websites[MAX_WEBSITES][MAX_LINE];
    int webCount = 0;

    while (fgets(websites[webCount], MAX_LINE, check) && webCount < MAX_WEBSITES) {
        strcpy(websites[webCount], trim(websites[webCount]));  // Trim whitespaces
        webCount++;
    }

    WebsiteCount specified[MAX_WEBSITES] = {0};
    for (int i = 0; i < webCount; i++) {
        strcpy(specified[i].website, websites[i]);
    }


    WebsiteCount unspecified[MAX_WEBSITES] = {0};
    int unSpecCount = 0;
    int domainURLCounter[2] = {0};
    int domainLoginCounter[2] = {0};

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), csv)) {
        Data data;
        sscanf(line, "\"%[^\"]\",\"%[^\"]\",\"%[^\"]\"", data.url, data.login, data.password);
        
        trim(data.url);  // Trim whitespaces
        int found = 0;
        for (int i = 0; i < webCount; i++) {
            if (strstr(data.url, websites[i])) {
                found = 1;
                specified[i].counter++;
                break;
            }
        }

        if (!found) {
            char *domain_start = strstr(data.url, "://");
            domain_start = domain_start ? domain_start + 3 : data.url;
            char domain[MAX_LINE] = {0};
            sscanf(domain_start, "%[^/]", domain);

            if (strlen(domain) == 0) continue;  // Skip if domain is empty

            int exists = 0;
            for (int j = 0; j < unSpecCount; j++) {
                if (strcmp(unspecified[j].website, domain) == 0) {
                    unspecified[j].counter++;
                    exists = 1;
                    break;
                }
            }
            if (!exists) {
                strcpy(unspecified[unSpecCount].website, domain);
                unspecified[unSpecCount++].counter = 1;
            }
        }

        if (strstr(data.url, ".ru")) domainURLCounter[0]++;
        if (strstr(data.url, ".su")) domainURLCounter[1]++;
        if (strstr(data.login, ".ru")) domainLoginCounter[0]++;
        if (strstr(data.login, ".su")) domainLoginCounter[1]++;
    }

    // Block 1
    fprintf(html, "<h2>Specified Websites</h2>");
    fprintf(html, "<table><tr><th>Website</th><th>Count</th></tr>");
    for (int i = 0; i < webCount; i++) {
        fprintf(html, "<tr><td>%s</td><td>%d</td></tr>", specified[i].website, specified[i].counter);
    }
    fprintf(html, "</table>");

    // Block 2
    fprintf(html, "<h2>Unspecified Websites</h2>");
    fprintf(html, "<table><tr><th>Website</th><th>Count</th></tr>");
    for (int i = 0; i < unSpecCount; i++) {
        fprintf(html, "<tr><td>%s</td><td>%d</td></tr>", unspecified[i].website, unspecified[i].counter);
    }
    fprintf(html, "</table>");

    // Block 3
    fprintf(html, "<h2>Domain Counters</h2>");
    fprintf(html, "<table><tr><th>Domain</th><th>URL Count</th><th>Login Count</th></tr>");
    fprintf(html, "<tr><td>.ru</td><td>%d</td><td>%d</td></tr>", domainURLCounter[0], domainLoginCounter[0]);
    fprintf(html, "<tr><td>.su</td><td>%d</td><td>%d</td></tr>", domainURLCounter[1], domainLoginCounter[1]);
    fprintf(html, "</table>");

    // Block 4
    int i;  // Declare loop variable here
    for (i = 0; i < webCount; i++) {
        fseek(csv, 0, SEEK_SET);  // Reset the CSV file pointer
        fflush(csv);  // Ensure the file stream is properly handled
        fprintf(html, "<h2>Entries for %s</h2>", specified[i].website);
        fprintf(html, "<table><tr><th>URL</th><th>Login</th><th>Password</th></tr>");
        Data data;  // Declare the data variable here
        while (fgets(line, sizeof(line), csv)) {
            memset(&data, 0, sizeof(data));  // Clear the buffer
            sscanf(line, "\"%[^\"]\",\"%[^\"]\",\"%[^\"]\"", data.url, data.login, data.password);
            if (strstr(data.url, specified[i].website)) {
                fprintf(html, "<tr><td>%s</td><td>%s</td><td>%s</td></tr>", data.url, data.login, data.password);
            }
        }
        fprintf(html, "</table>");
    }
    

    fprintf(html, "</body></html>");
    fclose(check);
    fclose(csv);
    fclose(html);

    return 0;
}
