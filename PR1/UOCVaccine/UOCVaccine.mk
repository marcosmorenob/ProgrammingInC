##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=UOCVaccine
ConfigurationName      :=Debug
WorkspacePath          :=/media/sf_PrProg/UOC20212
ProjectPath            :=/media/sf_PrProg/UOC20212/UOCVaccine
IntermediateDirectory  :=../build-$(ConfigurationName)
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=uoc
Date                   :=01/04/22
CodeLitePath           :=/home/uoc/.codelite
LinkerName             :=gcc
SharedObjectLinkerName :=gcc -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=../lib/lib$(ProjectName)d.a
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="UOCVaccine.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := gcc
CC       := gcc
CXXFLAGS :=  -g $(Preprocessors)
CFLAGS   :=  -g $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_person.c$(ObjectSuffix) $(IntermediateDirectory)/src_vaccine.c$(ObjectSuffix) $(IntermediateDirectory)/src_date.c$(ObjectSuffix) $(IntermediateDirectory)/src_csv.c$(ObjectSuffix) $(IntermediateDirectory)/src_api.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList) $(ArLibs)
	@$(MakeDirCommand) "/media/sf_PrProg/UOC20212/.build-debug"
	@echo rebuilt > "/media/sf_PrProg/UOC20212/.build-debug/UOCVaccine"

MakeIntermediateDirs:
	@test -d ../build-$(ConfigurationName) || $(MakeDirCommand) ../build-$(ConfigurationName)


../build-$(ConfigurationName):
	@test -d ../build-$(ConfigurationName) || $(MakeDirCommand) ../build-$(ConfigurationName)

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_person.c$(ObjectSuffix): src/person.c $(IntermediateDirectory)/src_person.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/media/sf_PrProg/UOC20212/UOCVaccine/src/person.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_person.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_person.c$(DependSuffix): src/person.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_person.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_person.c$(DependSuffix) -MM src/person.c

$(IntermediateDirectory)/src_person.c$(PreprocessSuffix): src/person.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_person.c$(PreprocessSuffix) src/person.c

$(IntermediateDirectory)/src_vaccine.c$(ObjectSuffix): src/vaccine.c $(IntermediateDirectory)/src_vaccine.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/media/sf_PrProg/UOC20212/UOCVaccine/src/vaccine.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_vaccine.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_vaccine.c$(DependSuffix): src/vaccine.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_vaccine.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_vaccine.c$(DependSuffix) -MM src/vaccine.c

$(IntermediateDirectory)/src_vaccine.c$(PreprocessSuffix): src/vaccine.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_vaccine.c$(PreprocessSuffix) src/vaccine.c

$(IntermediateDirectory)/src_date.c$(ObjectSuffix): src/date.c $(IntermediateDirectory)/src_date.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/media/sf_PrProg/UOC20212/UOCVaccine/src/date.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_date.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_date.c$(DependSuffix): src/date.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_date.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_date.c$(DependSuffix) -MM src/date.c

$(IntermediateDirectory)/src_date.c$(PreprocessSuffix): src/date.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_date.c$(PreprocessSuffix) src/date.c

$(IntermediateDirectory)/src_csv.c$(ObjectSuffix): src/csv.c $(IntermediateDirectory)/src_csv.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/media/sf_PrProg/UOC20212/UOCVaccine/src/csv.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_csv.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_csv.c$(DependSuffix): src/csv.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_csv.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_csv.c$(DependSuffix) -MM src/csv.c

$(IntermediateDirectory)/src_csv.c$(PreprocessSuffix): src/csv.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_csv.c$(PreprocessSuffix) src/csv.c

$(IntermediateDirectory)/src_api.c$(ObjectSuffix): src/api.c $(IntermediateDirectory)/src_api.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/media/sf_PrProg/UOC20212/UOCVaccine/src/api.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_api.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_api.c$(DependSuffix): src/api.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_api.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_api.c$(DependSuffix) -MM src/api.c

$(IntermediateDirectory)/src_api.c$(PreprocessSuffix): src/api.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_api.c$(PreprocessSuffix) src/api.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ../build-$(ConfigurationName)/


