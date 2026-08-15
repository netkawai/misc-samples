# OpenCode Configuration and Session Management

## Configuration Files (`opencode.json`)

OpenCode loads configuration from multiple locations in this order:

### Search Order
1. **Environment variable** (`OPENCODE_CONFIG_CONTENT`): Inline JSON config
2. **Custom file** (`OPENCODE_CONFIG` env): Specific file path
3. **Custom directory** (`OPENCODE_CONFIG_DIR` env): Directory containing config files
4. **Project config** (walking up):
   - `opencode.json` or `opencode.jsonc` files
   - `.opencode/` directories
   - Starts from current directory → parent directories → worktree root
5. **Home directory**: `$XDG_CONFIG_HOME/opencode/opencode.json` (typically `~/.config/opencode/`)
6. **Well-known remote config**: From authenticated providers
7. **Global config**: `~/.config/opencode/opencode.json`

### Path Locations
- **Global config**: `~/.config/opencode/opencode.json`
- **Local/project config**: `./opencode.json`, `./opencode.jsonc`, or `./.opencode/opencode.json` in your project
- **Custom directory**: `$OPENCODE_CONFIG_DIR/opencode.json`

### Example Setup
```bash
# Global settings (~/.config/opencode/opencode.json)
{
  "$schema": "https://opencode.ai/config.json",
  "model": "anthropic/claude-sonnet-4-6",
  "username": "yourname"
}

# Project settings (./opencode.json)
{
  "$schema": "https://opencode.ai/config.json",
  "model": "openai/gpt-4o",
  "skills": {
    "paths": [".opencode/skills"]
  }
}
```

## Default Model Selection

OpenCode determines the default AI model using:

1. **Configured model** from `opencode.json`: `"model": "provider/model-id"`
2. **Fallback to "opencode" provider's** first available model
3. **Latest available model** sorted by release date
4. **Configured model** (if valid but provider disabled)

Key code locations:
- `packages/opencode/src/acp/service.ts:789-813`: `defaultModelFromConfig()`
- `packages/core/src/catalog.ts:252-267`: Catalog's default model selection

## Session Storage

### Database Files
Sessions are stored in SQLite databases:

| Database File | Used When |
|---------------|-----------|
| `opencode-dev.db` | Development/local builds (default) |
| `opencode.db` | Production releases ("latest", "beta", "prod" channels) |
| `opencode-{channel}.db` | Channel-specific (e.g., `opencode-beta.db`) |

**Default location**: `~/.local/share/opencode/`

**Override**: Set `OPENCODE_DB=/path/to/custom.db` environment variable

### Database Schema
Key tables (`packages/core/src/session/sql.ts`):
- `session`: Metadata (ID, title, directory, model, timestamps, archived flag)
- `session_message`: Messages within sessions
- `session_input`: Prompts and inputs
- `session_context_epoch`: Context snapshots
- `todo`: Task lists

## Listing Sessions

### CLI Basic Listing
```bash
# List active sessions (default)
opencode session list

# List with JSON output
opencode session list --format=json

# Limit to N most recent sessions
opencode session list --max-count=10
```

### Include Archived Sessions (Via API)
The CLI doesn't support `--archived` flag directly. Use these methods:

#### 1. HTTP API (if server running)
```bash
# Get all sessions including archived
curl "http://localhost:3000/api/session?archived=true" | jq

# Or using experimental endpoint
curl "http://localhost:3000/experimental/session?archived=true" | jq
```

#### 2. Direct Database Query
```bash
# Find your database
ls ~/.local/share/opencode/opencode*.db

# Query all sessions with SQLite
sqlite3 ~/.local/share/opencode/opencode-dev.db <<EOF
SELECT 
  id, 
  title, 
  datetime(time_created/1000, 'unixepoch') as created,
  datetime(time_updated/1000, 'unixepoch') as updated,
  datetime(time_archived/1000, 'unixepoch') as archived,
  CASE WHEN time_archived IS NULL THEN 'active' ELSE 'archived' END as status
FROM session 
ORDER BY time_updated DESC;
EOF
```

#### 3. Programmatic SDK Usage
```typescript
// list-sessions.ts
import { OpencodeClient } from "@opencode-ai/core";

async function listAllSessions() {
  const sdk = OpencodeClient.create({ baseURL: "http://localhost:3000" });
  
  // Active sessions only
  const active = await sdk.session.list({ archived: false });
  console.log("Active:", active.data?.length || 0);
  
  // Archived sessions only
  const archived = await sdk.session.list({ archived: true });
  console.log("Archived:", archived.data?.length || 0);
}

listAllSessions().catch(console.error);
```

### Archive Status Field
Sessions have a `time_archived` timestamp field:
- `null` or `undefined`: Active session
- Set to timestamp: Archived session
- Archived sessions excluded by default from listings

## Key Source Files

| File | Purpose |
|------|---------|
| `packages/opencode/src/config/config.ts` | Configuration loading logic |
| `packages/opencode/src/config/paths.ts` | Config file search paths |
| `packages/core/src/global.ts` | Path definitions (`~/.config/opencode/`, `~/.local/share/opencode/`) |
| `packages/core/src/database/database.ts` | Database path resolution |
| `packages/core/src/session.ts` | Session listing implementation |
| `packages/core/src/session/sql.ts` | Database schema |
| `packages/opencode/src/cli/cmd/session.ts` | CLI `session list` command |
| `packages/opencode/src/acp/service.ts:789-813` | Default model selection |
| `packages/core/src/catalog.ts:252-267` | Catalog default model logic |

## Tips
1. Multiple `opencode.json` files merge with project overriding global
2. Model format: `"provider/model-id"` (e.g., `"anthropic/claude-sonnet-4-6"`)
3. Database persists across restarts - sessions remain until explicitly deleted
4. Archived sessions are hidden from default listings but still in database
5. Use `OPENCODE_DB` env var to specify custom database location
6. Development builds use `opencode-dev.db` by default