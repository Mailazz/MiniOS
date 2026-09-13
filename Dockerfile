FROM alpine:latest AS builder
RUN apk add --no-cache g++ make
WORKDIR /app
COPY . .
RUN make
FROM alpine:latest
RUN apk add --no-cache libstdc++
WORKDIR /app
COPY --from=builder /app/minios /app/minios
CMD ["./minios"]
