# Docker Monitor

A lightweight web-based monitor for inspecting the status of Docker containers grouped by service. It connects to the Docker socket and exposes a UI at port 3002.

This tool is shared across 5G-MAG Docker-based projects (e.g. [rt-5gms-examples](https://github.com/5G-MAG/rt-5gms-examples), [rt-mbs-examples](https://github.com/5G-MAG/rt-mbs-examples)).

## Usage

The monitor is launched via the `docker-compose-monitor.yml` file included in this directory. Run the following command from your project's Docker setup folder, passing that project's `.env` file explicitly:

```bash
docker compose -f /absolute/path/to/rt-common-shared/docker-monitor/docker-compose-monitor.yml \
  --env-file .env up -d
```

Then open **http://localhost:3002** in your browser.

> [!IMPORTANT]
> `--env-file .env` is required. Docker Compose resolves the default `.env` file relative to the project directory — which defaults to the directory of the first `-f` file, i.e. this `docker-monitor` directory — and *not* relative to the directory you run the command from. Without `--env-file`, the version variables stay unset. The path given to `--env-file` is resolved relative to the current working directory.

> [!NOTE]
> The monitor is independent of your project stack. It reads container state from the Docker socket rather than over a Docker network, so it can be started before, after, or entirely without the project stack, and it discovers containers on any network.

## Environment variables

The following variables are read from the project's `.env` file and displayed in the monitor UI. Any variable that is unset or empty is displayed as `unknown`:

| Variable | Description |
|---|---|
| `OPEN5GS_VERSION` | Expected Open5GS version |
| `FIVEG_MAG_MBS_VERSION` | Expected 5G-MAG MBS component version |
| `MONGODB_VERSION` | Expected MongoDB version |

## Tear down

```bash
docker compose -f /absolute/path/to/rt-common-shared/docker-monitor/docker-compose-monitor.yml \
  --env-file .env down
```
