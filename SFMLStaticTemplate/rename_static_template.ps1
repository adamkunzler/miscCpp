$REPO_NAME = $args[0]
$SOLUTION_NAME = $args[1]
$PROJECT_NAME = $args[2]

Write-Output "Rename Template Project"


#
# clone repo from github
#
Write-Output "Clone repo from GitHub..."
git clone https://github.com/adamkunzler/SFMLStaticTemplate.git

#
# rename repo folder
#
Write-Output "Rename Repo Folder..."
Rename-Item -Path "SFMLStaticTemplate" -NewName "$REPO_NAME"

#
# rename solution file
#
Write-Output "Rename Solution File..."
Rename-Item -Path "$REPO_NAME\CppImGuidSfml\CppImGuidSfml.sln" -NewName "$SOLUTION_NAME.sln"

#
# rename project files
#
Write-Output "Rename Project Files..."
Rename-Item -Path "$REPO_NAME\CppImGuidSfml\CppImGuidSfml\CppImGuidSfml.vcxproj" -NewName "$PROJECT_NAME.vcxproj"
Rename-Item -Path "$REPO_NAME\CppImGuidSfml\CppImGuidSfml\CppImGuidSfml.vcxproj.filters" -NewName "$PROJECT_NAME.vcxproj.filters"

#
#rename project and solution folders
#
Write-Output "Rename Project and Solution Folders..."
Rename-Item -Path "$REPO_NAME\CppImGuidSfml\CppImGuidSfml" -NewName "$PROJECT_NAME"
Rename-Item -Path "$REPO_NAME\CppImGuidSfml" -NewName "$SOLUTION_NAME"

#
# replace project file contents
#
Write-Output "Replacing Project File Contents..."
((Get-Content -path $REPO_NAME\$SOLUTION_NAME\$PROJECT_NAME\$PROJECT_NAME.vcxproj -Raw) -replace 'CppImGuidSfml', $PROJECT_NAME) | Set-Content -Path $REPO_NAME\$SOLUTION_NAME\$PROJECT_NAME\$PROJECT_NAME.vcxproj

#
# replace solution file contents
#
Write-Output "Replacing Solution File Contents..."
((Get-Content -path $REPO_NAME\$SOLUTION_NAME\$SOLUTION_NAME.sln -Raw) -replace '"CppImGuidSfml"', "`"$PROJECT_NAME`"") | Set-Content -Path $REPO_NAME\$SOLUTION_NAME\$SOLUTION_NAME.sln

((Get-Content -path $REPO_NAME\$SOLUTION_NAME\$SOLUTION_NAME.sln -Raw) -replace '"CppImGuidSfml\\CppImGuidSfml.vcxproj"', "`"$PROJECT_NAME\$PROJECT_NAME.vcxproj`"") | Set-Content -Path $REPO_NAME\$SOLUTION_NAME\$SOLUTION_NAME.sln

#
# remove .git folder
#
Write-Output "Remove .git Folder..."
Remove-Item "$REPO_NAME\.git" -Recurse -Force