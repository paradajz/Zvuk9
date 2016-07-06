#!/bin/bash
major=$(git tag -l "v*.0" | wc -l)
last_major_tag=$(git for-each-ref --format='%(*creatordate:raw)%(creatordate:raw) %(refname) %(*objectname) %(objectname)' refs/tags | sort -r | awk '{ print $3 }' | sed -e 's/refs\/tags\///g' | sed '/v*.0/q' | sort -r | head -1)
tags_since_major=$(git for-each-ref --format='%(*creatordate:raw)%(creatordate:raw) %(refname) %(*objectname) %(objectname)' refs/tags | sort -r | awk '{ print $3 }' | sed -e 's/refs\/tags\///g' | sed '/v*.0/Q' | wc -l)
last_tag_commit=$(git for-each-ref --format='%(*creatordate:raw)%(creatordate:raw) %(refname) %(*objectname) %(objectname)' refs/tags | sort -r | awk '{ print $4 }' | sed -e 's/refs\/tags\///g' | head -1)
commits_since_last_tag=$(git rev-list --remotes | sed '/'$last_tag_commit'/Q' | wc -l)
echo "last major version: $major"
echo "last major tag: $last_major_tag"
echo "tags since major version: $tags_since_major"
echo "last tag commit: $last_tag_commit"
echo "commits since last tag: $commits_since_last_tag"
echo "software version: $major.$tags_since_major.$commits_since_last_tag"
echo "$major" > "major"
echo "$tags_since_major" > "minor"
echo "$commits_since_last_tag" > "revision"