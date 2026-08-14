# `logic_agent` protocol schemas

`logic_agent` (`logic_agent/agent.cpp`) is a single-threaded JSON-RPC 2.0 server: it
reads one Request object at a time from STDIN and writes one Response object to STDOUT,
but *while* handling a request it also streams every liblogic listener callback that
fires along the way as a one-way JSON-RPC Notification, ahead of the final Response.
That gives the protocol two distinct shapes, documented in two separate files:

- **[`openrpc.json`](./openrpc.json)** — an [OpenRPC](https://spec.open-rpc.org/) 1.3.2
  document for the 5 request/response methods a client actually calls: `parse`,
  `transform`, `sat`, `truth_table`, `shutdown`. Method params/results and the AST
  (`Node`) shape they share live here. `shutdown` takes no params and, unlike the other
  four, ends the server's request loop right after its Response is sent.
- **[`feedback-events.schema.json`](./feedback-events.schema.json)** — a plain
  [JSON Schema](https://json-schema.org/) (2020-12) document, *not* OpenRPC, because
  OpenRPC's method model is request/response and these are fire-and-forget
  notifications. It catalogues all ~104 distinct `"<category>/<callbackName>"`
  notifications logic_agent can stream (lexer/parser/transform/sat/mapper/truth_table
  callbacks) and their payload shapes — `shutdown` streams none of its own, it's a bare
  control command. `$defs.AnyFeedbackNotification` validates any one parsed line of the
  stream without knowing its method name up front.

Both files are self-contained (no `$ref`s between them) — the handful of DTOs both need
(`Uuid`, `Lexeme`, `Node`, ...) are duplicated locally in each rather than shared, so
either can be fed to a validator/codegen tool on its own.

## Example exchange

Request (`parse`, relaxed/default parser):

```json
{"jsonrpc":"2.0","method":"parse","params":{"formula":"A \\wedge B"},"id":1}
```

Notifications streamed while handling it (one JSON value per STDOUT line, matched by
`feedback-events.schema.json`) — lexing first, then parsing:

```json
{"jsonrpc":"2.0","method":"lexer/didStart","params":{}}
{"jsonrpc":"2.0","method":"lexer/didRecogniseLexeme","params":{"lexeme":{"type":"Atom","token":"A","start":0,"end":1}}}
{"jsonrpc":"2.0","method":"lexer/didFinish","params":{}}
{"jsonrpc":"2.0","method":"parser/didStart","params":{}}
{"jsonrpc":"2.0","method":"parser/didJoin","params":{"target":null,"source":{"id":"...","type":"Atom","lexeme":{"type":"Atom","token":"A","start":0,"end":1},"children":[]}}}
{"jsonrpc":"2.0","method":"parser/didFinish","params":{}}
```

Final Response (matched by `openrpc.json`'s `parse` method result):

```json
{"jsonrpc":"2.0","id":1,"result":{"tree":{"id":"...","type":"AndOp","lexeme":{"type":"AndOp","token":"\\wedge","start":2,"end":8},"children":["...","..."]}}}
```

## Source of truth

Both schemas were hand-derived from, and should be kept in sync with:

- `logic_agent/rpc/dispatcher.cpp` — the method registry
- `logic_agent/rpc/methods/*_method.{hpp,cpp}` — per-method params/result shapes
- `logic_agent/types/rpc_request.{hpp,cpp}`, `logic_agent/types/rpc_response.{hpp,cpp}`,
  `logic_agent/rpc/rpc_exception.hpp` — the envelope and error codes
- `logic_agent/types/core/*.hpp`, `logic_agent/types/parser/cursor_t.hpp`,
  `logic_agent/types/sat/common_t.hpp` — the shared DTOs (`Uuid`, `Lexeme`, `Node`, ...)
- `logic_agent/types/**/*_events.hpp` — feedback/notification payload DTOs
- `logic_agent/rpc/streaming_*_listener.cpp` — the exact `"category/callbackName"`
  strings each DTO is streamed under (the source of truth for notification method names)
