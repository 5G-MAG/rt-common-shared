# Docker Monitor

A lightweight web-based monitor for inspecting the status of Docker containers grouped by service. It connects to the Docker socket and exposes a UI at port 3002.

This tool is shared across 5G-MAG Docker-based projects (e.g. [rt-5gms-examples](https://github.com/5G-MAG/rt-5gms-examples), [rt-mbs-examples](https://github.com/5G-MAG/rt-mbs-examples)).

## Usage

The monitor is launched via the `docker-compose-monitor.yml` file included in this directory. Run the following command from your project's Docker setup folder, so that Docker Compose picks up the project's `.env` file automatically:

```bash
docker compose -f /absolute/path/to/rt-common-shared/docker-monitor/docker-compose-monitor.yml up -d
```

Then open **http://localhost:3002** in your browser.

> [!NOTE]
> The monitor requires the `5g-mag` Docker network to exist. Start your project stack first before launching the monitor.

## Environment variables

The following variables are read from the project's `.env` file and displayed in the monitor UI:

| Variable | Description |
|---|---|
| `OPEN5GS_VERSION` | Expected Open5GS version |
| `FIVEG_MAG_MBS_VERSION` | Expected 5G-MAG MBS component version |
| `MONGODB_VERSION` | Expected MongoDB version |

## Tear down

```bash
docker compose -f /absolute/path/to/rt-common-shared/docker-monitor/docker-compose-monitor.yml down
```
