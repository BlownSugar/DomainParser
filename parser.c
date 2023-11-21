#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define DELIMITERS " ;:"

void trimNewline(char* str) {
    char* pos;
    if ((pos = strchr(str, '\n')) != NULL) {
        *pos = '\0';
    }
}

char determineDelimiter(const char* line) {
    const char delimiters[] = ";: ";
    for (int i = 0; line[i]; i++) {
        for (int j = 0; delimiters[j]; j++) {
            if (line[i] == delimiters[j]) {
                return delimiters[j];
            }
        }
    }
    return 0; // return 0 if no delimiter found
}

void extractURL(const char *line, char *url) {
    int i = 0, j = 0;
    
    // Skip the protocol
    while (line[i] && line[i] != ':' && line[i] != '/') {
        url[j++] = line[i++];
    }
    
    // Skip '://'
    while (line[i] && (line[i] == ':' || line[i] == '/')) {
        url[j++] = line[i++];
    }
    
    // Now, capture everything until space, ;, or :
    while (line[i] && line[i] != ' ' && line[i] != ';' && line[i] != ':') {
        url[j++] = line[i++];
    }
    url[j] = '\0';
}

int is_ru_or_su_domain(const char *url) {
    char *pos_ru = strstr(url, ".ru");
    char *pos_su = strstr(url, ".su");

    if (pos_ru && (pos_ru[3] == '/' || pos_ru[3] == ' ' || pos_ru[3] == ';' || pos_ru[3] == ':' || pos_ru[3] == '\0')) {
        return 1;
    }
    if (pos_su && (pos_su[3] == '/' || pos_su[3] == ' ' || pos_su[3] == ';' || pos_su[3] == ':' || pos_su[3] == '\0')) {
        return 1;
    }
    return 0;
}


int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Unable to open the input file");
        return 1;
    }

    FILE *output = fopen("output.csv", "w");
    if (!output) {
        perror("Unable to open the output file");
        fclose(file);
        return 1;
    }

    fprintf(output, "URL,Login,Password\n");

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        trimNewline(line);

        char url[MAX_LINE_LENGTH] = {0}, login[MAX_LINE_LENGTH] = {0}, password[MAX_LINE_LENGTH] = {0};

        printf("\nProcessing line: %s\n", line);

        // Extract URL
        extractURL(line, url);

        // Ensure URL contains ".ru" or ".su"
        if (!is_ru_or_su_domain(url)) {
            printf("Skipping as URL doesn't contain .ru or .su: %s\n", url);
            continue;
        }



        // Tokenize remainder of the line for login and password
        char *remainder = line + strlen(url);
        char *ptr = strtok(remainder, DELIMITERS);
        if (ptr) strncpy(login, ptr, sizeof(login) - 1);

        ptr = strtok(NULL, DELIMITERS);
        if (ptr) strncpy(password, ptr, sizeof(password) - 1);

        // Check if all fields are non-empty
        if (!*url || !*login || !*password) {
        printf("Error: One or more fields are missing!\n");
        continue;
    }

    printf("Parsed URL: %s, Login: %s, Password: %s\n", url, login, password);
    fprintf(output, "\"%s\",\"%s\",\"%s\"\n", url, login, password);
    fflush(output); // flush the stream to ensure the line is written immediately

}


    fclose(file);
    fclose(output);
    return 0;
}
