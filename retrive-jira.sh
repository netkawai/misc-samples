#!/bin/bash
# Usage:
#   chmod u+x ./snippet.sh
#   bash ./snippet.sh

# 1. Issue your API Token at https://id.atlassian.com/manage/api-tokens
# 2. Install "jq" command and add it to your bash's PATH https://stedolan.github.io/jq/
# 3. Modify the JQL below
HOST="host"
USER="email"
API_TOKEN="key"

# https://developer.atlassian.com/cloud/jira/platform/rest/#api-api-2-search-post

curl -sL --request POST \
  --user "${USER}:${API_TOKEN}" \
  --header 'Accept: application/json' \
  --header 'Content-Type: application/json' \
  --url "https://${HOST}/rest/api/2/search" \
  --data '{
    "jql": "project = AEGIS AND issuetype in (Precondition, Test) and attachments IS NOT EMPTY",
    "startAt": 0,
    "maxResults": 100,
    "fields": ["attachment"],
    "fieldsByKeys": false
  }' \
| jq .issues[]?.fields?.attachment[]?.content \
| xargs -I{} sh -c 'curl -L --user $1 --url $2 --output $(date +%s)_$(basename $2)' - "${USER}:${API_TOKEN}" {}
