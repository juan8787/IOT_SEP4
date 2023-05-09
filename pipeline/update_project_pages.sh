#! /bin/sh
set -e

if [ ! $# -eq 1 ]; then
    echo "Called with the a wrong number of arguments, expected 1 got $#"
    echo "$@"
    exit 1
fi

REPORAW="${1}"
REPOSITORY=$(echo "${1}" | tr  '[:upper:]' '[:lower:]' )
REPO_OWNER=$(echo "${REPOSITORY}" | cut -f1 -d/ )
REPO_NAME=$(echo "${REPOSITORY}" | cut -f2 -d/ )
PAGES_URL="https://${REPO_OWNER}.github.io/${REPO_NAME}/"

echo "Update project pages variables:"
echo "REPOSITORY = ${REPOSITORY}"
echo "REPO_OWNER = ${REPO_OWNER}"
echo "REPO_NAME = ${REPO_NAME}"
echo "PAGES_URL = ${PAGES_URL}"

COMMIT_NAME=$(git log -1 --pretty=format:%s)
UPDATE_TIME=$(date +"%Y-%m-%d %H:%M:%S")
README_CONTENT="## Latest Update\n\n- ${COMMIT_NAME} (${UPDATE_TIME})\n\nVisit ${PAGES_URL} to view the project pages."
if ! grep -q "${PAGES_URL}" README.md || [ ! -f README.md ]; then
    echo "Creating empty README.md file..."
    touch README.md
fi
echo "${README_CONTENT}" > README.md

# Update firmware.zip
rm -f proj_pages/firmware/firmware.zip
zip proj_pages/firmware/firmware.zip firmware.*

# Update test_result.xml
cp -f test_result.xml proj_pages/testdata/test_result.xml

# Update date stamps
NOW=$(date)
sed "s/TTTTTTTTTTTTTT/${NOW}/g" < proj_pages/index.html.template > index_tmp.html
sed "s:RRRRRRRRRRRRRR:${REPORAW}:g" < index_tmp.html > index.html
mv -f index.html proj_pages/index.html

exit 0
