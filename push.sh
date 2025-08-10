#!/bin/bash
set -e

PROJECT_ID="core-registry-458622"
REGION="us-west1"
REPO="core-repo"
IMAGE_NAME="grpc-log-aggregator"
TAG="latest"

# Full Artifact Registry path
IMAGE_URI="${REGION}-docker.pkg.dev/${PROJECT_ID}/${REPO}/${IMAGE_NAME}:${TAG}"

echo "[+] Building Docker image..."
docker build -t "${IMAGE_URI}" .

echo "[+] Authenticating with GCP..."
gcloud auth configure-docker "${REGION}-docker.pkg.dev"

echo "[+] Pushing image to Artifact Registry..."
docker push "${IMAGE_URI}"

echo "[✓] Push complete: ${IMAGE_URI}"
