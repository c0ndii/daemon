extern int copyOrNot(struct stat sourceFile, struct stat destFile);
extern int copyFile(char *fileSourcePath, char *fileDestPath, char *size, struct stat inputFileAttrib);
extern int copyBigFile(char *fileSourcePath, char *fileDestPath);
extern int copySmallFile(char *fileSourcePath, char *fileDestPath);