FROM python:3.14-slim

# Install build tools required for C++ compilation
RUN apt-get update \
    && apt-get install -y --no-install-recommends g++ make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

COPY . .

RUN mkdir -p output && g++ -std=c++17 -O2 -o output/main.exe main.cpp

EXPOSE 5000

CMD ["python", "app.py"]
